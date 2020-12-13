#include "levers.hpp"

#include <entt/entt.hpp>

#include "../comp/position.hpp"
#include "../comp/player.hpp"
#include "../comp/iob.hpp"
#include "../comp/collider.hpp"
#include "../comp/animation.hpp"
#include "../comp/entity_state.hpp"
#include "../util/vec_math.hpp"

using namespace aco::comp;

void aco::sys::toggle_lever(entt::registry& reg, sf::Keyboard::Key key_code)
{
	if (key_code != sf::Keyboard::Space)
	{
		return;
	}

	auto players{ reg.view<player, position, collider>() };
	auto levers{ reg.view<lever_switch, position, animation>() };

	for (const auto p : players)
	{
		const auto p_pos = players.get<position>(p).pos;
		const auto p_collider = players.get<collider>(p);
		const sf::Vector2f p_center = p_pos + p_collider.offset + p_collider.size / 2.0f;

		for (const auto l : levers)
		{
			const auto l_pos = levers.get<position>(l).pos;
			const auto l_size = levers.get<animation>(l).frame_size;
			const sf::Vector2f l_center = l_pos + l_size / 2.0f;

			const auto activation_distance = levers.get<lever_switch>(l).activation_distance;

			if (util::distance(p_center, l_center) < activation_distance)
			{
				if (reg.remove_if_exists<switch_on>(l) == 0)
				{
					reg.emplace<switch_on>(l);
				}
			}
		}
	}
}

void aco::sys::activate_lever(entt::registry& reg)
{
	auto levers{ reg.view<lever_switch, switch_on>() };
	for (const auto l : levers)
	{
		reg.emplace_or_replace<active_state>(l);

		const auto& torches = levers.get<lever_switch>(l).torches;
		for (const auto t : torches)
		{
			if (reg.remove_if_exists<active_state>(t) == 0)
			{
				reg.emplace<active_state>(t);
			}
		}
	}
}
