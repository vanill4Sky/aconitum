#include "render.hpp"

#include <algorithm>
#include <entt/entt.hpp>

using namespace aco::comp;

void aco::sys::sort_sprites(std::vector<const sprite*>& orderd_sprites)
{
	static const auto compare = [](const sprite* a, const sprite* b) {
		const auto& a_spr{ a->spr };
		const auto& b_spr{ b->spr };
		const auto& bounds_a{ a_spr.getGlobalBounds() };
		const auto& bounds_b{ b_spr.getGlobalBounds() };
		auto pos_a{ a_spr.getPosition().y };
		if (!a->is_ground_object)
		{
			pos_a += bounds_a.height;
		}
		auto pos_b{ b_spr.getPosition().y };
		if (!b->is_ground_object)
		{
			pos_b += bounds_b.height;
		}

		return pos_a < pos_b;
	};

	std::sort(orderd_sprites.begin(), orderd_sprites.end(), compare);
}

void aco::sys::draw_entities(std::vector<const sprite*>& orderd_sprites, sf::RenderWindow& window)
{	
	for (const auto* sprite : orderd_sprites)
	{
		window.draw(sprite->spr);
	}
}