#pragma once

#include <SFML/System/Vector2.hpp>

namespace aco::comp
{

struct animation 
{
	animation(sf::Vector2f frame_size)
		: frame_size{ frame_size }
	{}
	animation(float frame_width, float frame_height)
		: frame_size{ frame_width, frame_height }
	{}
	animation()
		: animation(0.0f, 0.0f)
	{}
	sf::Vector2f frame_size;
};

}