#pragma once

#include <entt/fwd.hpp>

namespace aco::sys
{

void find_next_position(entt::registry& reg);
void player_iob_collide(entt::registry& reg);
void submit_next_position(entt::registry& reg);

}