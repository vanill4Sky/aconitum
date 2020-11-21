#include "factories.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <sol/sol.hpp>
#include <spdlog/spdlog.h>

#include "../comp/components.hpp"
#include "sprite_picker.hpp"
#include "../core/dir.hpp"

using namespace aco::comp;

entt::id_type aco::create_entity(entt::registry& reg, resource_holder<sf::Texture>& textures,
	const sol::table& entity_template, const sf::Vector2f& pos, size_t idx)
{
	const auto e{ reg.create() };
	for (const auto& pair : entity_template)
	{
		const auto comp_name{ pair.first.as<std::string>() };
		if (comp_name == "sprite")
		{
			auto s{ pair.second.as<sprite>() };
			textures.load(s.tex_path);
			s.spr.setTexture(textures.get(s.tex_path));
			reg.emplace<sprite>(e, s);
		}
		else if (comp_name == "position")
		{
			reg.emplace<position>(e, pair.second.as<position>());
		}
		else if (comp_name == "next_position")
		{
			reg.emplace<next_position>(e, pair.second.as<next_position>());
		}
		else if (comp_name == "iob")
		{
			reg.emplace<iob>(e, pair.second.as<iob>());
		}
		else if (comp_name == "box")
		{
			reg.emplace<box>(e, pair.second.as<box>());
		}
		else if (comp_name == "pressure_plate")
		{
			reg.emplace<pressure_plate>(e, pair.second.as<pressure_plate>());
		}
		else if (comp_name == "player")
		{
			reg.emplace<player>(e, pair.second.as<player>());
		}
		else if (comp_name == "animation")
		{
			reg.emplace<animation>(e, pair.second.as<animation>());
		}
		else if (comp_name == "velocity")
		{
			reg.emplace<velocity>(e, pair.second.as<velocity>());
		}
		else if (comp_name == "collider")
		{
			reg.emplace<collider>(e, pair.second.as<collider>());
		}
		else if (comp_name == "kinematic")
		{
			reg.emplace<kinematic>(e, pair.second.as<kinematic>());
		}
		else if (comp_name == "trigger")
		{
			reg.emplace<trigger>(e, pair.second.as<trigger>());
		}
		else if (comp_name == "mob")
		{
			reg.emplace<mob>(e, pair.second.as<mob>());
		}
		else if (comp_name == "target")
		{
			reg.emplace<target>(e, pair.second.as<target>());
		}
		else if (comp_name == "stalker")
		{
			reg.emplace<stalker>(e, pair.second.as<stalker>());
		}
		else if (comp_name == "idle_state")
		{
			reg.emplace<idle_state>(e, pair.second.as<idle_state>());
		}
		else
		{
			spdlog::warn("Unhandled component: {}", comp_name);
		}
	}

	return static_cast<entt::id_type>(e);
}
