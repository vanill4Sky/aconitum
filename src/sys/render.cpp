#include "render.hpp"

#include <entt/entt.hpp>

#include "../comp/player.hpp"
#include "../comp/position.hpp"
#include "../comp/direction.hpp"
#include "../comp/sprite.hpp"
#include "../comp/entity_state.hpp"
#include "../app/sprite_picker.hpp"

using namespace aco::comp;

void aco::sys::draw_player(entt::registry& reg, sf::RenderWindow& window, const size_t frame_cnt)
{
	const auto view{ reg.view<player, position, direction, sprite>() };
	for (const auto e : view)
	{
		const auto player_dir{ view.get<direction>(e).dir };
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
