#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace aco::comp
{

struct sprite
{
	sprite(std::string tex_path, bool is_ground_object)
		: spr{ }
		, tex_path{ tex_path }
		, is_ground_object{ is_ground_object } {}
	sprite(std::string tex_path)
		: sprite(tex_path, false) {}
	sprite(sf::Sprite spr)
		: spr{ spr }
		, tex_path{ "" }
		, is_ground_object{ false } {}
	sprite()
		: sprite("") {}

	sf::Sprite spr;
	std::string tex_path;
	bool is_ground_object;
};

}