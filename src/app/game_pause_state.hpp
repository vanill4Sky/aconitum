#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "../core/state.hpp"
#include "app_data.hpp"
#include "button.hpp"

namespace aco
{

class game_pause_state : public state
{
public:
	game_pause_state(aco::app_data& app_data);
	void init();
	void handle_input();
	void update(float dt);
	void draw();

private:
	void set_layout();

	aco::app_data& m_app_data;
	sf::Sprite m_background;
	aco::button m_resume;
	aco::button m_restart;
	aco::button m_back_to_main_menu;
	std::vector<aco::button*> m_widgets;
};

}