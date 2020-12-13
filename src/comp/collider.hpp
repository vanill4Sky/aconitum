#pragma once

#include <SFML/System/Vector2.hpp>

namespace aco::comp
{

struct collider 
{
	collider(sf::Vector2f offset, sf::Vector2f size)
		: offset{ offset }, size{ size }
	{}
	collider(float left, float top, float width, float height)
		: offset{ left, top }, size{ width, height }
	{}
	collider()
		: collider(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	sf::Vector2f offset;
	sf::Vector2f size;
};

struct trigger {};

struct kinematic {};

}