#include "doors.hpp"

#include <entt/entt.hpp>
#include <sol/sol.hpp>

#include "../comp/components.hpp"

using namespace aco::comp;

void aco::sys::open_doors(entt::registry& reg, sol::state& state)
{
	sol::optional<sol::function> door_open_opt = state["door_open"];
	if (door_open_opt == sol::nullopt)
	{
		return;
	}
	const auto door_open = door_open_opt.value();

	const auto view = reg.view<door>();
	for (const auto e : view)
	{
		if (door_open())
		{
			reg.emplace_or_replace<trigger>(e);
			reg.emplace_or_replace<active_state>(e);
		}
		else
		{
			reg.remove_if_exists<trigger, active_state>(e);
		}
	}
}
