#pragma once 

#include <entt/fwd.hpp>

namespace aco::sys
{

void animate_mob(entt::registry& reg, size_t frame_cnt);
void animate_iob(entt::registry& reg, size_t frame_cnt);

}