#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "../comp/sprite.hpp"

namespace aco::sys
{

void fill_orderd_sprites(entt::registry& reg, std::vector<const sf::Sprite*>& ordered_sprites);

}