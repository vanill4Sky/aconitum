#include "render.hpp"

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

void aco::sys::draw_entities(entt::registry& reg, sf::RenderWindow& window, const size_t frame_cnt)
{	
	const auto view{ reg.view<position, sprite>() };
	for (const auto e : view)
	{
		auto& spr{ view.get<sprite>(e).spr };
		spr.setPosition(view.get<position>(e).pos);

		if (reg.has<animation>(e))
		{
			const auto dir{ reg.get<velocity>(e).dir };
			if (reg.has<move_state>(e))
			{
				spr.setTextureRect(
					aco::pick_sprite(64.0f, aco::pose::walkcycle, dir, (frame_cnt / 4) % 9));
			}
			else
			{
				spr.setTextureRect(
					aco::pick_sprite(64.0f, aco::pose::walkcycle, dir, 0));
			}
		}

		window.draw(spr);
	}
}