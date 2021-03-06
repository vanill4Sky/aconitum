#include "sprite_picker.hpp"

#include <SFML/System/Vector2.hpp>

sf::IntRect aco::pick_sprite(sf::Vector2i size, int row, int col)
{
	return sf::IntRect{ sf::Vector2i{ col * size.x, row * size.y }, size };
}

sf::IntRect aco::pick_sprite(sf::Vector2f size, aco::pose pose, aco::dir dir, int frame)
{
	int row{ static_cast<int>(pose) };

	switch (dir)
	{
	case aco::dir::up_right:
	case aco::dir::right:
	case aco::dir::down_right:
		row += 3;
		break;
	case aco::dir::down:
		row += 2;
		break;
	case aco::dir::down_left:
	case aco::dir::left:
	case aco::dir::up_left:
		row += 1;
		break;
	default:
		break;
	}

	return aco::pick_sprite(static_cast<sf::Vector2i>(size), row, frame);
}
