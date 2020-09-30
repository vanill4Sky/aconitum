#include "factories.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../comp/player.hpp"
#include "../comp/direction.hpp"
#include "../comp/position.hpp"
#include "../comp/sprite.hpp"
#include "../comp/mob.hpp"
#include "../comp/iob.hpp"
#include "../comp/target.hpp"
#include "../comp/animation.hpp"
#include "../comp/collider.hpp"
#include "sprite_picker.hpp"
#include "../core/dir.hpp"


entt::entity aco::create_player(entt::registry& reg, const sf::Texture& texture)
{
	sf::Sprite sprite{ texture, aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0) };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::player>(e);
	reg.emplace<aco::comp::animation>(e);
	reg.emplace<aco::comp::velocity>(e, aco::dir::none, 3.0f);
	reg.emplace<aco::comp::position>(e);
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
	reg.emplace<aco::comp::position>(e);
	reg.emplace<aco::comp::next_position>(e, sf::Vector2f{ 100.0f, 100.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::collider>(e, sf::Vector2f{ 20.0f, 44.0f }, sf::Vector2f{ 24.0f, 18.0f });
	reg.emplace<aco::comp::target>(e, target, 250.0f, 50.0f);
}

void aco::create_box(entt::registry& reg, const sf::Texture& texture)
{
	sf::Sprite sprite{ texture };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::iob>(e);
	reg.emplace<aco::comp::box>(e);
	reg.emplace<aco::comp::position>(e);
	reg.emplace<aco::comp::next_position>(e, sf::Vector2f{ 600.0f, 500.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::collider>(e, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 23.0f, 31.0f });
}
