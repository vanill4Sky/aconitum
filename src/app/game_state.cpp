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
#include "../sys/triggers.hpp"
#include "../sys/levers.hpp"
#include "../sys/doors.hpp"
#include "../sys//game_over.hpp"
#include "../util/file.hpp"
#include "../util/graphics.hpp"
#include "constants.hpp"
#include "lua_binding.hpp"

#include "editor_state.hpp"
#include "game_pause_state.hpp"
#include "game_over_state.hpp"

aco::game_state::game_state(aco::app_data& app_data, bool restart)
	: m_app_data{ app_data }
	, m_current_level{ 32.0f }
	, m_view{ m_app_data.window.getDefaultView() }
{
	update_levels_list();
	m_view.zoom(game_view_zoom);

	if (!restart)
	{
		m_current_level_idx = util::read_from_file(save_path, 0);
	}
}

aco::game_state::~game_state()
{
	util::write_to_file(m_current_level_idx, save_path);
}

void aco::game_state::init()
{
	register_factory(m_app_data.lua, m_reg, m_app_data.textures);
	register_modifiers(m_app_data.lua, m_reg);
	register_queries(m_app_data.lua, m_reg);

	m_app_data.fonts.load(arial_path);
	m_level_info.setFont(m_app_data.fonts.get(arial_path));
	m_level_info.setCharacterSize(20);
	update_level_info();

	load_level(m_level_files_list[m_current_level_idx]);
	resize_view();
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
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_app_data.state_manager.push_state(std::make_unique<aco::game_pause_state>(m_app_data));
			}

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
					update_levels_list();
					load_level(m_level_files_list[m_current_level_idx]);
					break;
				case sf::Keyboard::F9:
					m_app_data.state_manager.push_state(std::make_unique<editor_state>(m_app_data));
					break;
				}
			}

			if (aco::sys::key_pressed(m_reg, m_keyboard_state, event.key.code))
			{
				aco::sys::toggle_lever(m_reg, event.key.code);
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			aco::sys::key_released(m_reg, m_keyboard_state, event.key.code);
		}
		else if (event.type == sf::Event::Resized)
		{
			sf::Vector2u new_size{ event.size.width, event.size.height };
			m_view = util::resize_view(m_view, new_size);
			m_view.zoom(game_view_zoom);
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
	mob_iob_collide(m_reg);
	mob_wall_collide(m_reg, m_current_level);
	submit_next_position(m_reg);
	update_pressure_plates(m_reg);
	activate_lever(m_reg);
	open_doors(m_reg, m_app_data.lua);

	if (check_loss(m_reg))
	{
		m_app_data.state_manager.replace_state(std::make_unique<game_over_state>(m_app_data, false));
	}
	else if (check_win(m_reg))
	{
		if (m_current_level_idx == (m_level_files_list.size() - 1))
		{
			m_app_data.state_manager.replace_state(std::make_unique<game_over_state>(m_app_data, true));
		}
		else
		{
			load_level(m_level_files_list[++m_current_level_idx]);
			update_level_info();
		}
		return;
	}

	center_view_on_player(m_reg, m_view);
	sort_sprites(m_ordered_sprites);
	animate_mob(m_reg, frame_cnt);
	animate_iob(m_reg, frame_cnt);

	clear_triggers(m_reg);

	m_app_data.window.setView(m_view);
	m_level_info.setPosition(m_app_data.window.mapPixelToCoords({ 10, 10 }));

	++frame_cnt;
}

void aco::game_state::draw()
{	
	m_app_data.window.clear();

	m_current_level.draw(m_app_data.window, false);
	aco::sys::draw_entities(m_ordered_sprites, m_app_data.window);
	m_app_data.window.draw(m_level_info);

	m_app_data.window.display();
}

void aco::game_state::resume()
{
	if (m_app_data.go_back_to_main_menu)
	{
		m_app_data.state_manager.pop_state();
		m_app_data.go_back_to_main_menu = false;
	}
	else if (m_app_data.restart_level)
	{
		load_level(m_level_files_list[m_current_level_idx]);
		m_app_data.restart_level = false;
	}

	resize_view();
}

void aco::game_state::update_levels_list()
{
	m_level_files_list.clear();
	m_app_data.lua.script_file(level_list_dir);
	sol::optional<sol::table> list_opt = m_app_data.lua["level_list"];
	assert(list_opt != sol::nullopt);

	const auto list = list_opt.value();
	for (size_t i = 0; i <= list.size(); ++i)
	{
		const std::string level_name = list[i];
		m_level_files_list.emplace_back(level_name + ".json");
	}
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

void aco::game_state::update_level_info()
{
	m_level_info.setString(std::to_string(m_current_level_idx + 1) + 
		"/" + std::to_string(m_level_files_list.size()));
}

void aco::game_state::resize_view()
{
	sf::Vector2u size{ m_app_data.window.getSize() };
	m_view = util::resize_view(m_view, size);
	m_view.zoom(game_view_zoom);
}
