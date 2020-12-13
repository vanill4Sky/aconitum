#pragma once

#include <entt/fwd.hpp>

#include "../app/level.hpp"

namespace aco::sys
{
	void localize_target(entt::registry& reg, const aco::level& level);

	void localize_target(entt::registry& reg);
	void target_triggers(entt::registry& reg);
}