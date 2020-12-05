#pragma once

#include <entt/fwd.hpp>

namespace aco
{

bool set_target(entt::registry& reg, entt::entity self, entt::entity target);
bool connect_torch(entt::registry& reg, entt::entity self, entt::entity torch);

}