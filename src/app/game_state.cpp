#include "game_state.hpp"

#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <cassert>

#include "../sys/player_input.hpp"
#include "../sys/movement.hpp"
#include "../sys/render.hpp"
#include "../sys/target_following.hpp"
#include "../sys/level_init.hpp"
#include "../sys/animation.hpp"
#include "constants.hpp"
#include "lua_binding.hpp"
#include "../util/file.hpp"

#include "editor_state.hpp"

aco::game_state::game_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, m_current_level{ 32.0f }
	, m_view{ m_app_data.window.getDefaultView() }
{
	update_levels_list();
	m_view.zoom(0.5);
}

void aco::game_state::init()
{
	register_factory(m_app_data.lua, m_reg, m_app_data.textures);
	register_modifiers(m_app_data.lua, m_reg);
}


void aco::game_state::handle_input()
{
	sf::Event event;
	while (m_app_data.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_app_data.window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (!m_level_files_list.empty())
			{
				switch (event.key.code)
				{
				case sf::Keyboard::F5:
					load_level(m_level_files_list[++m_current_level_idx %= m_level_files_list.size()]);
					break;
				case sf::Keyboard::F6:
					load_level(m_level_files_list[--m_current_level_idx %= m_level_files_list.size()]);
					break;
				case sf::Keyboard::F8:
					load_level(m_level_files_list[m_current_level_idx]);
					break;
				case sf::Keyboard::F9:
					m_app_data.state_manager.push_state(std::make_unique<editor_state>(m_app_data));
					break;
				}
			}

			aco::sys::key_pressed(m_reg, m_keyboard_state, event.key.code);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			aco::sys::key_released(m_reg, m_keyboard_state, event.key.code);
		}
	}
}

void aco::game_state::update(float dt)
{
	using namespace aco::sys;

	localize_target(m_reg);
	target_triggers(m_reg);
	find_next_position(m_reg);
	player_iob_collide(m_reg, m_current_level);
	player_wall_collide(m_reg, m_current_level);
	submit_next_position(m_reg, m_view);
	sort_sprites(m_ordered_sprites);
	animate_mob(m_reg, frame_cnt);
	animate_iob(m_reg, frame_cnt);

	m_app_data.window.setView(m_view);

	++frame_cnt;
}

void aco::game_state::draw()
{	
	m_app_data.window.clear();

	m_current_level.draw(m_app_data.window, false);
	aco::sys::draw_entities(m_ordered_sprites, m_app_data.window);

	m_app_data.window.display();
}

void aco::game_state::update_levels_list()
{
	m_level_files_list = util::list_filenames(levels_dir, "json");
}

void aco::game_state::load_level(const std::string& level_name)
{
	m_reg.clear();

	m_current_level.read_from_file(levels_dir + level_name);
	if (std::string level_script_path{ level_scripts_dir + util::replace_extension(level_name, ".lua") };
		util::exists_not_empty(level_script_path))
	{
		m_app_data.lua.script_file(level_script_path);
	}
	else
	{
		spdlog::error("Level script file {} does not exisits.", level_script_path);
	}

	aco::sys::fill_orderd_sprites(m_reg, m_ordered_sprites);
}
