#pragma once

#include <SFML/Graphics/View.hpp>
#include <entt/fwd.hpp>

namespace aco
{

class level;

}

namespace aco::sys
{

void find_next_position(entt::registry& reg);
void player_iob_collide(entt::registry& reg);
void player_wall_collide(entt::registry& reg, const aco::level& level);
void submit_next_position(entt::registry& reg, sf::View& render_view);

}