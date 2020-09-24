#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "../core/dir.hpp"

namespace aco
{

enum class pose : int
{
	spellcast = 0,
	thrust = 4,
	walkcycle = 8,
	slash = 12,
	shoot = 16,
	hurt = 20
};

sf::IntRect pick_sprite(int size, int row, int col);
sf::IntRect pick_sprite(int size, aco::pose pose, aco::dir dir, int frame);

}