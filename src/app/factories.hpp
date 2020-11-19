#pragma once

#include <entt/fwd.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <sol/forward.hpp>

#include "../core/resource_holder.hpp"


namespace aco
{

entt::id_type create_entity(entt::registry& reg, resource_holder<sf::Texture>& textures, 
	const sol::table& entity_template, const sf::Vector2f& pos, size_t idx);

}