#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "../core/state.hpp"
#include "app_data.hpp"
#include "button.hpp"

namespace aco
{

class main_menu_state : public state
{
public:
	main_menu_state(aco::app_data& app_data);
	void init();
	void handle_input();
	void update(float dt);
	void draw();
	void resume();

private:
	void set_layout();

	aco::app_data& m_app_data;
	sf::Sprite m_background;
	aco::button m_header;
	aco::button m_start_button;
	aco::button m_continue_button;
	aco::button m_help_button;
	aco::button m_about_button;
	aco::button m_editor_button;
	aco::button m_exit_button;
	std::vector<aco::button*> m_widgets;
};

}