#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "../core/state.hpp"
#include "app_data.hpp"
#include "button.hpp"

namespace aco
{

class game_over_state : public state
{
public:
	game_over_state(aco::app_data& app_data, bool has_win);
	void init();
	void handle_input();
	void update(float dt);
	void draw();

private:
	void set_layout();

	aco::app_data& m_app_data;
	bool m_has_win;
	sf::Sprite m_background;
	aco::button m_retry;
	aco::button m_back_to_main_menu;
	std::vector<aco::button*> m_widgets;
};

}