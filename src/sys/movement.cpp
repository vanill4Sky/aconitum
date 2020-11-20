#include "movement.hpp"

#include <entt/entt.hpp>

#include "../util/vec_math.hpp"
#include "../comp/direction.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/position.hpp"
#include "../comp/player.hpp"
#include "../comp/iob.hpp"
#include "../comp/collider.hpp"
#include "../comp/sprite.hpp"
#include "../comp/animation.hpp"
#include "../app/level.hpp"

#include <spdlog/spdlog.h>

using namespace aco::comp;

namespace aco::sys
{

bool iob_iob_collide(entt::registry& reg, entt::entity e)
{
	auto iobs{ reg.view<iob, position, next_position, collider>() };
	assert(iobs.contains(e));

	const auto e_curr_pos{ iobs.get<position>(e).pos };
	const auto e_next_pos{ iobs.get<next_position>(e).pos };
	const auto& e_collider{ iobs.get<collider>(e) };
	sf::FloatRect e_bbox{ e_next_pos + e_collider.offset, e_collider.size };

	for (const auto i : iobs)
	{
		if (i == e)
		{
			continue;
		}

		const auto i_curr_pos{ iobs.get<position>(i).pos };
		const auto& i_collider{ iobs.get<collider>(i) };
		sf::FloatRect i_bbox{ i_curr_pos + i_collider.offset, i_collider.size };

		if (e_bbox.intersects(i_bbox))
		{
			return true;
		}
	}

	return false;
}

}

void aco::sys::find_next_position(entt::registry& reg)
{
	auto view{ reg.view<velocity, position, next_position, move_state>() };
	for (const auto e : view)
	{
		const auto v{ view.get<velocity>(e) };
		const auto current_pos{ view.get<position>(e).pos };
		view.get<next_position>(e).pos = current_pos + aco::to_vec2<float>(v.dir) * v.speed;
	}
}

bool has_collision(const aco::level& level, const sf::FloatRect& bbox)
{
	const float tile_size{ level.tile_size() };
	const auto [x, y, w, h] = bbox;
	return level.at(aco::layer::bottom, x / tile_size, y / tile_size).is_collidable
		|| level.at(aco::layer::bottom, (x + w) / tile_size, y / tile_size).is_collidable
		|| level.at(aco::layer::bottom, x / tile_size, (y + h) / tile_size).is_collidable
		|| level.at(aco::layer::bottom, (x + w) / tile_size, (y + h) / tile_size).is_collidable;
}

void aco::sys::player_iob_collide(entt::registry& reg, const aco::level& level)
{
	auto players{ reg.view<player, position, next_position, collider>() };
	auto iobs{ reg.view<iob, position, next_position, collider>() };

	for (const auto p : players)
	{
		const auto p_curr_pos{ players.get<position>(p).pos };
		auto& p_next_pos{ players.get<next_position>(p).pos };
		const auto& p_collider{ players.get<collider>(p) };
		const sf::FloatRect p_bbox{ p_collider.offset + p_next_pos, p_collider.size };
		const sf::FloatRect p_bbox_horizontal{ 
			p_collider.offset + sf::Vector2f{ p_next_pos.x, p_curr_pos.y }, p_collider.size };
		const sf::FloatRect p_bbox_vertical{ 
			p_collider.offset + sf::Vector2f{ p_curr_pos.x, p_next_pos.y }, p_collider.size };

		for (const auto i : iobs)
		{
			const auto i_curr_pos{ iobs.get<position>(i).pos };
			auto& i_next_pos{ iobs.get<next_position>(i).pos };
			const auto& i_collider{ iobs.get<collider>(i) };
			sf::FloatRect i_bbox{ i_curr_pos + i_collider.offset, i_collider.size };

			if (reg.has<trigger>(i))
			{
				if (p_bbox.intersects(i_bbox))
				{
					reg.emplace_or_replace<active_state>(i);
				}
				else
				{
					reg.remove_if_exists<active_state>(i);
				}
			}
			else if (reg.has<kinematic>(i))
			{
				if (p_bbox_horizontal.intersects(i_bbox))
				{
					p_next_pos.x = p_curr_pos.x;
				}
				if (p_bbox_vertical.intersects(i_bbox))
				{
					p_next_pos.y = p_curr_pos.y;
				}
			}
			else
			{
				sf::Vector2<bool> is_collision;
				if (is_collision.x = p_bbox_horizontal.intersects(i_bbox))
				{
					const auto offset{ p_next_pos.x - p_curr_pos.x };
					i_next_pos.x += offset;
					i_bbox.left += offset;
				}
				else if (is_collision.y = p_bbox_vertical.intersects(i_bbox))
				{
					const auto offset{ p_next_pos.y - p_curr_pos.y };
					i_next_pos.y += offset;
					i_bbox.top += offset;
				}
				else
				{
					continue;
				}

				if (has_collision(level, i_bbox) || iob_iob_collide(reg, i))
				{
					if (is_collision.x)
					{
						i_next_pos.x = i_curr_pos.x;
						p_next_pos.x = p_curr_pos.x;
					}
					else if (is_collision.y)
					{
						i_next_pos.y = i_curr_pos.y;
						p_next_pos.y = p_curr_pos.y;
					}
				}
			}
		}
	}
}

void aco::sys::player_wall_collide(entt::registry& reg, const aco::level& level)
{
	auto players{ reg.view<position, next_position, collider, velocity>() };
	for (const auto p : players)
	{
		const auto p_curr_pos{ players.get<position>(p).pos };
		auto& p_next_pos{ players.get<next_position>(p).pos };
		const auto& p_collider{ players.get<collider>(p) };

		if (has_collision(level, sf::FloatRect{
			p_collider.offset + sf::Vector2f{ p_next_pos.x, p_curr_pos.y }, p_collider.size }))
		{
			p_next_pos.x = p_curr_pos.x;
		}
		if (has_collision(level, sf::FloatRect{
			p_collider.offset + sf::Vector2f{ p_curr_pos.x, p_next_pos.y }, p_collider.size }))
		{
			p_next_pos.y = p_curr_pos.y;
		}
	}
}

void aco::sys::submit_next_position(entt::registry& reg, sf::View& render_view)
{
	auto players{ reg.view<player, position, next_position>() };
	for (const auto p : players)
	{
		const auto next_pos{ players.get<next_position>(p).pos };
		auto& curr_pos{ players.get<position>(p).pos };
		curr_pos = next_pos;

		if (reg.has<sprite>(p))
		{
			reg.get<sprite>(p).spr.setPosition(next_pos);
		}

		render_view.setCenter(curr_pos);
	}

	auto view{ reg.view<position, next_position>(entt::exclude<player>) };
	for (const auto e : view)
	{
		const auto next_pos{ view.get<next_position>(e).pos };
		auto& curr_pos{ view.get<position>(e).pos };
		curr_pos = next_pos;

		if (reg.has<sprite>(e))
		{
			reg.get<sprite>(e).spr.setPosition(next_pos);
		}
	}
}
