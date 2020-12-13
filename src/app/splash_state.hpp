#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../core/state.hpp"
#include "app_data.hpp"

namespace aco
{

class splash_state : public state
{
public:
	splash_state(aco::app_data& app_data);
	void init();
	void handle_input();
	void update(float dt);
	void draw();

private:
	aco::app_data& m_app_data;
	sf::Clock m_timer;
	sf::Sprite m_logo;
};

}