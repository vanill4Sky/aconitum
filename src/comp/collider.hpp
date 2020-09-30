#pragma once

#include <SFML/System/Vector2.hpp>

namespace aco::comp
{

struct collider 
{
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