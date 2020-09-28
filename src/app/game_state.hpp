#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../core/state.hpp"
#include "app_data.hpp"
#include "../core/dir.hpp"

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
	app_data& m_app_data;
	entt::registry m_reg;
	std::unordered_map <sf::Keyboard::Key, bool> m_keyboard_state;

	sf::Texture player_tex;
	sf::Texture stalker_tex;
	aco::dir player_dir{ aco::dir::none };
	unsigned int frame_cnt{ 0 };
};

}