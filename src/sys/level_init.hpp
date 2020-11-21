#pragma once

#include <entt/fwd.hpp>
#include <vector>

#include "../comp/sprite.hpp"

namespace aco::sys
{

void fill_orderd_sprites(entt::registry& reg, std::vector<const aco::comp::sprite*>& ordered_sprites);

}