#include "render.hpp"

#include <algorithm>
#include <entt/entt.hpp>

#include "../comp/player.hpp"
#include "../comp/position.hpp"
#include "../comp/direction.hpp"
#include "../comp/sprite.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/mob.hpp"
#include "../comp/animation.hpp"
#include "../app/sprite_picker.hpp"

using namespace aco::comp;

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

	//const auto view{ reg.view<sprite>() };
	//for (const auto e : view)
	//{
	//	auto& spr{ view.get<sprite>(e).spr };

	//	if (reg.has<animation>(e))
	//	{
	//		const auto dir{ reg.get<velocity>(e).dir };
	//		if (reg.has<move_state>(e))
	//		{
	//			spr.setTextureRect(
	//				aco::pick_sprite(64.0f, aco::pose::walkcycle, dir, (frame_cnt / 4) % 9));
	//		}
	//		else
	//		{
	//			spr.setTextureRect(
	//				aco::pick_sprite(64.0f, aco::pose::walkcycle, dir, 0));
	//		}
	//	}
	//}
}