#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

#include "../core/state_manager.hpp"

namespace aco
{

class app_data
{
public:
	static aco::app_data& get_instance();

	app_data(const aco::app_data&) = delete;
	void operator=(const aco::app_data&) = delete;

	sf::RenderWindow window;
	aco::state_manager state_manager;

private:
	app_data();
};

}