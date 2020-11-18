#include "factories.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <sol/sol.hpp>
#include <spdlog/spdlog.h>

#include "../comp/components.hpp"
#include "sprite_picker.hpp"
#include "../core/dir.hpp"

using namespace aco::comp;

entt::entity aco::create_player(entt::registry& reg, const sf::Texture& texture)
{
	sf::Sprite sprite{ texture, aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0) };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::player>(e);
	reg.emplace<aco::comp::animation>(e);
	reg.emplace<aco::comp::velocity>(e, aco::dir::none, 3.0f);
	reg.emplace<aco::comp::position>(e, sf::Vector2f{ 500.0f, 500.0f });
	reg.emplace<aco::comp::next_position>(e, sf::Vector2f{ 500.0f, 500.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::collider>(e, sf::Vector2f{ 20.0f, 44.0f }, sf::Vector2f{ 24.0f, 18.0f });

	return e;
}

void aco::create_stalker(entt::registry& reg, const entt::entity target, const sf::Texture& texture)
{
	sf::Sprite sprite{ texture, aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0) };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::mob>(e);
	reg.emplace<aco::comp::stalker>(e);
	reg.emplace<aco::comp::animation>(e);
	reg.emplace<aco::comp::velocity>(e, aco::dir::none, 1.5f);
	reg.emplace<aco::comp::position>(e, sf::Vector2f{ 150.0f, 100.0f });
	reg.emplace<aco::comp::next_position>(e, sf::Vector2f{ 150.0f, 100.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::collider>(e, sf::Vector2f{ 20.0f, 44.0f }, sf::Vector2f{ 24.0f, 18.0f });
	reg.emplace<aco::comp::target>(e, target, 250.0f, 50.0f);
}

void aco::create_box(entt::registry& reg, const sf::Texture& texture, sf::Vector2f position)
{
	sf::Sprite sprite{ texture };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::iob>(e);
	reg.emplace<aco::comp::box>(e);
	reg.emplace<aco::comp::position>(e);
	reg.emplace<aco::comp::next_position>(e, position);
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::collider>(e, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 23.0f, 31.0f });
}

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
		else
		{
			spdlog::warn("Unhandled component: {}", comp_name);
		}
	}

	if (reg.has<sprite, animation>(e))
	{
		auto& s{ reg.get<sprite>(e) };
		s.spr.setTextureRect(aco::pick_sprite(64.0f, aco::pose::walkcycle, aco::dir::down_left, 0));
	}

	return static_cast<entt::id_type>(e);
}
