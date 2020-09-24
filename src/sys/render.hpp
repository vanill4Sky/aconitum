#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace aco::sys
{

void draw_player(entt::registry& reg, sf::RenderWindow& window, const size_t frame_cnt);

}