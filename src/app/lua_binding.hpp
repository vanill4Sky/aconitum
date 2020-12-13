#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <sol/forward.hpp>

#include "../core/resource_holder.hpp"

namespace aco
{

void register_user_types(sol::state& state);
void register_factory(sol::state& state, entt::registry& reg, resource_holder<sf::Texture>& textures);
void register_modifiers(sol::state& state, entt::registry& reg);
void register_queries(sol::state& state, entt::registry& reg);

}