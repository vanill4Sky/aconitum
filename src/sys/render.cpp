#include "render.hpp"

#include <algorithm>
#include <entt/entt.hpp>

void aco::sys::sort_sprites(std::vector<const sf::Sprite*>& orderd_sprites)
{
	static const auto compare = [](const sf::Sprite* a, const sf::Sprite* b) {
		const auto& bounds_a{ a->getGlobalBounds() };
		const auto& bounds_b{ b->getGlobalBounds() };
		const auto& pos_a{ a->getPosition().y + bounds_a.height };
		const auto& pos_b{ b->getPosition().y + bounds_b.height };

		return pos_a < pos_b;
	};

	std::sort(orderd_sprites.begin(), orderd_sprites.end(), compare);
}

void aco::sys::draw_entities(std::vector<const sf::Sprite*>& orderd_sprites, sf::RenderWindow& window)
{	
	for (const auto* sprite : orderd_sprites)
	{
		window.draw(*sprite);
	}
}