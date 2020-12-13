#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

namespace aco
{
	class button;
}

namespace util
{

void scale_sprite(sf::Sprite& sprite, sf::Vector2f desired_size);
void center_sprite(sf::Sprite& sprite, sf::Vector2u window_size);
void center_origin_of_sprite(sf::Sprite& sprite);
sf::View resize_view(const sf::View& view, sf::Vector2u new_size);
void column_layout(aco::button& widget, unsigned int window_width, float offset);
void column_layout(sf::Sprite& widget, unsigned int window_width, float offset);

template <typename T>
void center_origin(T& drawable);

}

template<typename T>
void util::center_origin(T& drawable)
{
	const auto local_bounds = drawable.getLocalBounds();

	const sf::Vector2f new_origin{
		local_bounds.left + local_bounds.width / 2.0f,
		local_bounds.top + local_bounds.height / 2.0f };
	drawable.setOrigin(new_origin);
}

