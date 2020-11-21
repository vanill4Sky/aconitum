#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

#include "../comp/sprite.hpp"

namespace aco::sys
{

void sort_sprites(std::vector<const aco::comp::sprite*>& orderd_sprites);
void draw_entities(std::vector<const aco::comp::sprite*>& orderd_sprites, sf::RenderWindow& window);

}