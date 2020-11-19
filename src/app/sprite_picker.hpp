#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "../core/dir.hpp"

namespace aco
{

enum class pose : int
{
	walkcycle = 0,
};

sf::IntRect pick_sprite(int size, int row, int col);
sf::IntRect pick_sprite(int size, aco::pose pose, aco::dir dir, int frame);

}