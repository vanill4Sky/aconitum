#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace aco::comp
{

struct sprite
{
	sprite(std::string tex_path)
		: spr{ }
		, tex_path{ tex_path } {}
	sprite(sf::Sprite spr)
		: spr{ spr }
		, tex_path{ tex_path } {}
	sprite()
		: sprite("") {}

	sf::Sprite spr;
	std::string tex_path;
};

}