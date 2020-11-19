#include "animation.hpp"

#include <entt/entt.hpp>

#include "../comp/direction.hpp"
#include "../comp/sprite.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/animation.hpp"
#include "../app/sprite_picker.hpp"

using namespace aco::comp;

void aco::sys::animate_mob(entt::registry& reg, size_t frame_cnt)
{
	const auto view{ reg.view<sprite>() };
	for (const auto e : view)
	{
		auto& spr{ view.get<sprite>(e).spr };

		if (reg.has<animation>(e))
		{
			const auto frame_size{ static_cast<int>(reg.get<animation>(e).frame_size.x) };
			const auto dir{ reg.get<velocity>(e).dir };
			if (reg.has<move_state>(e))
			{
				spr.setTextureRect(
					aco::pick_sprite(frame_size, aco::pose::walkcycle, dir, (frame_cnt / 4) % 9));
			}
			else
			{
				spr.setTextureRect(
					aco::pick_sprite(frame_size, aco::pose::walkcycle, dir, 0));
			}
		}
	}
}
