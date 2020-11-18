#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../core/state.hpp"
#include "app_data.hpp"
#include "../core/dir.hpp"
#include "level.hpp"

namespace aco
{

class game_state : public state
{
public:
	game_state(aco::app_data& app_data);

	void init();
	void handle_input();
	void update(float dt);
	void draw();

private:
	void update_levels_list();
	void load_level(const std::string& level_name);

	app_data& m_app_data;
	entt::registry m_reg;
	std::unordered_map <sf::Keyboard::Key, bool> m_keyboard_state;
	level m_current_level;
	sf::View m_view;
	std::vector<std::string> m_level_files_list;
	size_t m_current_level_idx{ 0 };

	sf::Texture player_tex;
	sf::Texture stalker_tex;
	sf::Texture box_tex;
	aco::dir player_dir{ aco::dir::none };
	unsigned int frame_cnt{ 0 };
};

}