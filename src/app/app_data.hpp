#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <sol/sol.hpp>

#include "../core/state_manager.hpp"
#include "../core/resource_holder.hpp"

namespace aco
{

class app_data
{
public:
	static app_data& get_instance();

	app_data(const aco::app_data&) = delete;
	void operator=(const aco::app_data&) = delete;

	sf::RenderWindow window;
	aco::state_manager state_manager;
	aco::resource_holder<sf::Texture> textures;
	aco::resource_holder<sf::Font> fonts;
	sol::state lua;

private:
	app_data() {};
};

}