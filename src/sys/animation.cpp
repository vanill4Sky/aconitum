#include "animation.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "../comp/direction.hpp"
#include "../comp/sprite.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/animation.hpp"
#include "../comp/iob.hpp"
#include "../app/sprite_picker.hpp"

using namespace aco::comp;

void aco::sys::animate_mob(entt::registry& reg, size_t frame_cnt)
{
	const auto view{ reg.view<sprite, animation, velocity>() };
	for (const auto e : view)
	{
		auto& spr{ view.get<sprite>(e).spr };
		const auto& a{ view.get<animation>(e) };
		const auto dir{ view.get<velocity>(e).dir };
		if (reg.has<move_state>(e))
		{
			spr.setTextureRect(
				aco::pick_sprite(a.frame_size, aco::pose::walkcycle, dir, (frame_cnt / 4) % a.frame_count));
		}
		else
		{
			spr.setTextureRect(
				aco::pick_sprite(a.frame_size, aco::pose::walkcycle, dir, 0));
		}
	}
}

sf::IntRect pick_frame(sf::Vector2f frame_size, int row, int col)
{
	const auto size{ static_cast<sf::Vector2i>(frame_size) };

	return { { col * size.x, row * size.y }, size };
}

void aco::sys::animate_iob(entt::registry& reg, size_t frame_cnt)
{
	const auto view{ reg.view<iob, sprite, animation>() };
	for (const auto e : view)
	{
		auto& spr{ view.get<sprite>(e).spr };
		const auto& a{ view.get<animation>(e) };

		if (reg.has<active_state>(e))
		{
			spr.setTextureRect(pick_frame(a.frame_size, 0, a.frame_count - 1));
		}
		else if (reg.has<idle_state>(e))
		{
			spr.setTextureRect(pick_frame(a.frame_size, 0, (frame_cnt / 4) % a.frame_count));
		}
		else
		{
			spr.setTextureRect(pick_frame(a.frame_size, 0, 0));
		}
	}
}
