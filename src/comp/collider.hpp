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

	/// <summary>
	/// Offset relative to entity position
	/// </summary>
	sf::Vector2f offset;
	/// <summary>
	/// Size of rectangular collider
	/// </summary>
	sf::Vector2f size;
};

}