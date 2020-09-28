#include "target_following.hpp"

#include <cmath>
#include <entt/entt.hpp>

#include "../comp/position.hpp"
#include "../comp/target.hpp"
#include "../comp/direction.hpp"
#include "../comp/entity_state.hpp"

using namespace aco::comp;

void aco::sys::localize_target(entt::registry& reg)
{
	auto view{ reg.view<position, velocity, target>() };
	for (const auto e : view)
	{
		const auto current_pos{ view.get<position>(e).pos };
		const auto current_target{ view.get<target>(e) };
		const auto target_pos{ reg.get<position>(current_target.entity).pos };

		auto new_direction{ target_pos - current_pos };
		new_direction /= std::sqrt(new_direction.x * new_direction.x + new_direction.y * new_direction.y);
		new_direction.x += std::copysign(0.5f, new_direction.x);
		new_direction.y += std::copysign(0.5f, new_direction.y);

		view.get<velocity>(e).dir = aco::to_dir(new_direction);
	}
}

void aco::sys::target_triggers(entt::registry& reg)
{
	auto view{ reg.view<position, target>() };
	for (const auto e : view)
	{
		const auto current_pos{ view.get<position>(e).pos };
		const auto current_target{ view.get<target>(e) };
		const auto target_pos{ reg.get<position>(current_target.entity).pos };

		auto distance{ std::sqrt(std::pow(target_pos.y - current_pos.y, 2)
			+ std::pow(target_pos.x - current_pos.x, 2)) };

		if (distance > current_target.trigger_distance || distance < current_target.stop_distance)
		{
			reg.remove_if_exists<move_state>(e);
		}
		else
		{
			if(!reg.has<move_state>(e))
			{
				reg.emplace<move_state>(e);
			}
		}
	}
}
