#include "game_over.hpp"

#include <entt/entt.hpp>
#include "../comp/components.hpp"
#include "../util/vec_math.hpp"
#include "../app/constants.hpp"

using namespace aco::comp;

sf::Vector2f calc_center(const position& p, const collider& c)
{
	sf::Vector2f center = p.pos + c.offset + c.size / 2.0f;
	return center;
}

bool aco::sys::check_loss(entt::registry& reg)
{
	auto players{ reg.view<player, position, collider>() };
	auto stalkers{ reg.view<stalker, position, collider>() };
	for (const auto p : players)
	{
		const auto p_center = calc_center(players.get<position>(p), players.get<collider>(p));
		for (const auto s : stalkers)
		{
			const auto s_center = calc_center(stalkers.get<position>(s), stalkers.get<collider>(s));
			if (util::distance(p_center, s_center) < stalker_loss_distance)
			{
				return true;
			}
		}
	}

	return false;
}

bool aco::sys::check_win(entt::registry& reg)
{
	auto view{ reg.view<door>() };
	for (const auto e : view)
	{
		if (reg.has<player_at_door>(e))
		{
			return true;
		}
	}

    return false;
}
