#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

namespace aco::sys
{

void sort_sprites(std::vector<const sf::Sprite*>& orderd_sprites);
void draw_entities(std::vector<const sf::Sprite*>& orderd_sprites, sf::RenderWindow& window);

}