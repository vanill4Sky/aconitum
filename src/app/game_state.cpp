#include "game_state.hpp"

#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <cassert>

#include "sprite_picker.hpp"
#include "../sys/player_input.hpp"
#include "../sys/movement.hpp"
#include "../sys/render.hpp"
#include "../sys/target_following.hpp"
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

	/*
	player_tex.loadFromFile("assets/textures/player_thief_01.png");
	stalker_tex.loadFromFile("assets/textures/stalker_thief_01.png");
	box_tex.loadFromFile("assets/textures/box_01.png");

	const auto player_entity{ aco::create_player(m_reg, player_tex) };
	aco::create_stalker(m_reg, player_entity, stalker_tex);
	sf::Vector2f box_pos{ 600.0f, 0.0f };
	for (int i = 0; i < 10; ++i)
	{
		aco::create_box(m_reg, box_tex, box_pos);
		aco::create_box(m_reg, box_tex, box_pos + sf::Vector2f{ 100.0f, 0.0f });
		box_pos.y += 100.0f;
	}
	*/
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
					load_level(m_level_files_list[++m_current_level_idx % m_level_files_list.size()]);
						break;
				case sf::Keyboard::F6:
					load_level(m_level_files_list[--m_current_level_idx % m_level_files_list.size()]);
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
	aco::sys::localize_target(m_reg);
	aco::sys::target_triggers(m_reg);
	aco::sys::find_next_position(m_reg);
	aco::sys::player_iob_collide(m_reg);
	aco::sys::player_wall_collide(m_reg, m_current_level);
	aco::sys::submit_next_position(m_reg, m_view);
}

void aco::game_state::draw()
{
	m_app_data.window.setView(m_view);
	
	m_app_data.window.clear();

	m_current_level.draw(m_app_data.window, false);
	aco::sys::draw_entities(m_reg, m_app_data.window, frame_cnt);

	m_app_data.window.display();

	++frame_cnt;
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
}
