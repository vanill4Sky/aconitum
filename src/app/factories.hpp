#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace aco
{

entt::entity create_player(entt::registry& reg, const sf::Texture& texture);
void create_stalker(entt::registry& reg, const entt::entity target, const sf::Texture& texture);
void create_box(entt::registry& reg, const sf::Texture& texture);

}