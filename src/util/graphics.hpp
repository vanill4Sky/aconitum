#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

namespace util
{

void scale_sprite(sf::Sprite& sprite, sf::Vector2f desired_size);
void center_sprite(sf::Sprite& sprite, sf::Vector2u window_size);
void center_origin_of_sprite(sf::Sprite& sprite);
sf::View resize_view(const sf::View& view, sf::Vector2u new_size);

}