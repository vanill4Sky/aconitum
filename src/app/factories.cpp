#include "factories.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../comp/player.hpp"
#include "../comp/direction.hpp"
#include "../comp/position.hpp"
#include "../comp/sprite.hpp"
#include "../comp/creatures.hpp"
#include "../comp/target.hpp"
#include "sprite_picker.hpp"
#include "../core/dir.hpp"

entt::entity aco::create_player(entt::registry& reg, const sf::Texture& texture)
{
	sf::Sprite sprite{ texture, aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0) };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::player>(e);
	reg.emplace<aco::comp::velocity>(e, aco::dir::none, 3.0f);
	reg.emplace<aco::comp::position>(e, sf::Vector2f{ 500.0f, 500.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);

	return e;
}

void aco::create_stalker(entt::registry& reg, const sf::Texture& texture, const entt::entity target)
{
	sf::Sprite sprite{ texture, aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0) };

	const auto e{ reg.create() };
	reg.emplace<aco::comp::creature>(e);
	reg.emplace<aco::comp::stalker>(e);
	reg.emplace<aco::comp::velocity>(e, aco::dir::none, 1.5f);
	reg.emplace<aco::comp::position>(e, sf::Vector2f{ 100.0f, 100.0f });
	reg.emplace<aco::comp::sprite>(e, sprite);
	reg.emplace<aco::comp::target>(e, target, 500.0f, 50.0f);
}
