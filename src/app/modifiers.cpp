#include "modifiers.hpp"

#include <iostream>
#include <entt/entt.hpp>

#include "../comp/components.hpp"

bool aco::set_target(entt::registry& reg, entt::entity self, entt::entity target)
{
	if (reg.has<aco::comp::target>(self))
	{
		auto& t{ reg.get<aco::comp::target>(self) };
		t.entity = target;

		return true;
	}
	return false;
}

bool aco::connect_torch(entt::registry& reg, entt::entity self, entt::entity torch)
{
	if (reg.has<aco::comp::lever_switch>(self))
	{
		auto& torches{ reg.get<aco::comp::lever_switch>(self).torches };
		torches.push_back(torch);

		return true;
	}
	return false;
}
