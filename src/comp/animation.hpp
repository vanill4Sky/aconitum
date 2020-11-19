#pragma once

#include <SFML/System/Vector2.hpp>

namespace aco::comp
{

struct animation 
{
	animation(sf::Vector2f frame_size, int frame_count)
		: frame_size{ frame_size }
		, frame_count{ frame_count }
	{}
	animation(float frame_width, float frame_height, int frame_count)
		: animation{ {frame_width, frame_height}, frame_count }
	{}
	animation()
		: animation(0.0f, 0.0f, 0)
	{}
	sf::Vector2f frame_size;
	int frame_count;
};

}