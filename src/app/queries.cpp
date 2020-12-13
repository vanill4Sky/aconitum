#include "modifiers.hpp"

#include <iostream>
#include <entt/entt.hpp>

#include "../comp/entity_state.hpp"
#include "queries.hpp"

using namespace aco::comp;

bool aco::has_active_state(entt::registry& reg, entt::entity e)
{
	return reg.has<active_state>(e);
}
