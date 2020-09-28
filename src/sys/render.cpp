#include "render.hpp"

#include <entt/entt.hpp>

#include "../comp/player.hpp"
#include "../comp/position.hpp"
#include "../comp/direction.hpp"
#include "../comp/sprite.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/creatures.hpp"
#include "../app/sprite_picker.hpp"

using namespace aco::comp;

void aco::sys::draw_player(entt::registry& reg, sf::RenderWindow& window, const size_t frame_cnt)
{
	const auto view{ reg.view<player, position, velocity, sprite>() };
	for (const auto e : view)
	{
		const auto player_dir{ view.get<velocity>(e).dir };
		auto& player_sprite{ view.get<sprite>(e).spr };

		player_sprite.setPosition(view.get<position>(e).pos);

		if (reg.has<move_state>(e))
		{
			player_sprite.setTextureRect(
				aco::pick_sprite(64.0f, aco::pose::walkcycle, player_dir, (frame_cnt / 4) % 9));
		}
		else
		{
			player_sprite.setTextureRect(
				aco::pick_sprite(64.0f, aco::pose::walkcycle, player_dir, 0));
		}

		window.draw(player_sprite);
	}
}

void aco::sys::draw_creatures(entt::registry& reg, sf::RenderWindow& window, const size_t frame_cnt)
{
	const auto view{ reg.view<creature, position, velocity, sprite>() };
	for (const auto e : view)
	{
		const auto creature_dir{ view.get<velocity>(e).dir };
		auto& creature_sprite{ view.get<sprite>(e).spr };

		creature_sprite.setPosition(view.get<position>(e).pos);

		if (reg.has<move_state>(e))
		{
			creature_sprite.setTextureRect(
				aco::pick_sprite(64.0f, aco::pose::walkcycle, creature_dir, (frame_cnt / 4) % 9));
		}
		else
		{
			creature_sprite.setTextureRect(
				aco::pick_sprite(64.0f, aco::pose::walkcycle, creature_dir, 0));
		}

		window.draw(creature_sprite);
	}
}
