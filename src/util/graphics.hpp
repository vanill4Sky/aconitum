#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace util
{

void scale_sprite(sf::Sprite& sprite, sf::Vector2f desired_size)
{
	const auto sprite_global_bounds = sprite.getGlobalBounds();

	const sf::Vector2f logo_scale{
		desired_size.x / sprite_global_bounds.width,
		desired_size.y / sprite_global_bounds.height };
	sprite.scale(logo_scale);
}

void center_sprite(sf::Sprite& sprite, sf::Vector2u window_size)
{
	const sf::Vector2f new_pos = static_cast<sf::Vector2f>(window_size) / 2.0f;
	sprite.setPosition(new_pos);
}

void center_origin_of_sprite(sf::Sprite& sprite)
{
	const auto sprite_local_bounds = sprite.getLocalBounds();

	const sf::Vector2f new_origin{
		sprite_local_bounds.width / 2.0f,
		sprite_local_bounds.height / 2.0f };
	sprite.setOrigin(new_origin);
}

}