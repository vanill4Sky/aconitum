#pragma once

#include <SFML/System/Vector2.hpp>
#include <sol/forward.hpp>

namespace aco::comp
{

struct position
{
	position(sf::Vector2f pos)
		: pos{ pos }
	{}
	position(float x, float y)
		: pos{ x, y }
	{}
	position()
		: position(0.0f, 0.0f)
	{}

	sf::Vector2f pos;
};

struct next_position
{
	next_position(sf::Vector2f pos)
		: pos{ pos }
	{}
	next_position(float x, float y)
		: pos{ x, y }
	{}
	next_position()
		: next_position(0.0f, 0.0f)
	{}

	sf::Vector2f pos;
};

}