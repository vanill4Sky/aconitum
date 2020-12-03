#include "movement.hpp"

#include <entt/entt.hpp>

#include "../util/vec_math.hpp"
#include "../comp/direction.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/position.hpp"
#include "../comp/player.hpp"
#include "../comp/iob.hpp"
#include "../comp/mob.hpp"
#include "../comp/collider.hpp"
#include "../comp/sprite.hpp"
#include "../comp/animation.hpp"
#include "../app/level.hpp"

#include <spdlog/spdlog.h>

using namespace aco::comp;

namespace aco::sys
{

inline bool iob_iob_collide(entt::registry& reg, entt::entity e)
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
			if (reg.has<trigger>(i))
			{
				reg.emplace_or_replace<active_state>(i);
				return false;
			}

			return true;
		}
	}

	return false;
}

inline bool has_collision(const aco::level& level, const sf::FloatRect& bbox)
{
	const float tile_size{ level.tile_size() };
	const auto [x, y, w, h] = bbox;
	return level.at(aco::layer::bottom, x / tile_size, y / tile_size).is_collidable
		|| level.at(aco::layer::bottom, (x + w) / tile_size, y / tile_size).is_collidable
		|| level.at(aco::layer::bottom, x / tile_size, (y + h) / tile_size).is_collidable
		|| level.at(aco::layer::bottom, (x + w) / tile_size, (y + h) / tile_size).is_collidable;
}

inline sf::FloatRect calc_bbox(const sf::Vector2f& position, const collider& collider)
{
	return { collider.offset + position, collider.size };
}

inline std::pair<sf::FloatRect, sf::FloatRect> calc_next_bbox(
	const sf::Vector2f& current_pos, const sf::Vector2f& next_pos, const collider& collider)
{
	return { calc_bbox({ next_pos.x, current_pos.y }, collider), 
		calc_bbox({ current_pos.x, next_pos.y }, collider) };
}

inline void on_trigger_collision(
	entt::registry& reg, const sf::FloatRect& mob_bbox, const sf::FloatRect& iob_bbox, entt::entity iob)
{
	if (mob_bbox.intersects(iob_bbox))
	{
		reg.emplace_or_replace<active_state>(iob);
	}
}

inline void on_kinematic_collision(
	entt::registry& reg, const sf::Vector2f& mob_current_pos, sf::Vector2f& mob_next_pos,
	const collider& collider, const sf::FloatRect& iob_bbox)
{
	const auto [mob_bbox_horizontal, mob_bbox_vertical] = 
		calc_next_bbox(mob_current_pos, mob_next_pos, collider);
	if (mob_bbox_horizontal.intersects(iob_bbox))
	{
		mob_next_pos.x = mob_current_pos.x;
	}
	if (mob_bbox_vertical.intersects(iob_bbox))
	{
		mob_next_pos.y = mob_current_pos.y;
	}
}

inline void on_standard_collision(
	entt::registry& reg, const aco::level& level,
	const sf::Vector2f& mob_current_pos, sf::Vector2f& mob_next_pos, const collider& collider,
	const sf::Vector2f& iob_current_pos, sf::Vector2f& iob_next_pos, sf::FloatRect iob_bbox, entt::entity iob)
{
	const auto [mob_bbox_horizontal, mob_bbox_vertical] =
		calc_next_bbox(mob_current_pos, mob_next_pos, collider);

	sf::Vector2<bool> is_collision;
	if (const auto offset{ mob_next_pos - mob_current_pos };
		is_collision.x = mob_bbox_horizontal.intersects(iob_bbox))
	{
		iob_next_pos.x += offset.x;
		iob_bbox.left += offset.x;
	}
	else if (is_collision.y = mob_bbox_vertical.intersects(iob_bbox))
	{
		iob_next_pos.y += offset.y;
		iob_bbox.top += offset.y;
	}
	else
	{
		return;
	}

	if (has_collision(level, iob_bbox) || iob_iob_collide(reg, iob))
	{
		if (is_collision.x)
		{
			iob_next_pos.x = iob_current_pos.x;
			mob_next_pos.x = mob_current_pos.x;
		}
		else if (is_collision.y)
		{
			iob_next_pos.y = iob_current_pos.y;
			mob_next_pos.y = mob_current_pos.y;
		}
	}
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

void aco::sys::player_iob_collide(entt::registry& reg, const aco::level& level)
{
	auto players{ reg.view<player, position, next_position, collider>() };
	auto iobs{ reg.view<iob, position, next_position, collider>() };

	for (const auto p : players)
	{
		const auto p_curr_pos{ players.get<position>(p).pos };
		auto& p_next_pos{ players.get<next_position>(p).pos };
		const auto& p_collider{ players.get<collider>(p) };
		const sf::FloatRect p_bbox{ calc_bbox(p_next_pos, p_collider) };

		for (const auto i : iobs)
		{
			const auto i_curr_pos{ iobs.get<position>(i).pos };
			auto& i_next_pos{ iobs.get<next_position>(i).pos };
			const auto& i_collider{ iobs.get<collider>(i) };
			sf::FloatRect i_bbox{ calc_bbox(i_next_pos, i_collider) };

			if (reg.has<trigger>(i))
			{
				on_trigger_collision(reg, p_bbox, i_bbox, i);
			}
			else if (reg.has<kinematic>(i))
			{
				on_kinematic_collision(reg, p_curr_pos, p_next_pos, p_collider, i_bbox);
			}
			else
			{
				on_standard_collision(reg, level, p_curr_pos, p_next_pos, p_collider, 
					i_curr_pos, i_next_pos, i_bbox, i);
			}
		}
	}
}

void aco::sys::mob_iob_collide(entt::registry& reg)
{
	auto mobs{ reg.view<mob, position, next_position, collider>() };
	auto iobs{ reg.view<iob, position, next_position, collider>() };

	for (const auto m : mobs)
	{
		const auto m_curr_pos{ mobs.get<position>(m).pos };
		auto& m_next_pos{ mobs.get<next_position>(m).pos };
		const auto& m_collider{ mobs.get<collider>(m) };
		const sf::FloatRect m_bbox{ calc_bbox(m_next_pos, m_collider) };

		for (const auto i : iobs)
		{
			const auto i_curr_pos{ iobs.get<position>(i).pos };
			auto& i_next_pos{ iobs.get<next_position>(i).pos };
			const auto& i_collider{ iobs.get<collider>(i) };
			sf::FloatRect i_bbox{ calc_bbox(i_next_pos, i_collider) };

			if (reg.has<trigger>(i))
			{
				on_trigger_collision(reg, m_bbox, i_bbox, i);
			}
			else
			{
				on_kinematic_collision(reg, m_curr_pos, m_next_pos, m_collider, i_bbox);
			}
		}
	}
}

void aco::sys::mob_wall_collide(entt::registry& reg, const aco::level& level)
{
	auto mobs{ reg.view<position, next_position, collider, velocity>() };
	for (const auto p : mobs)
	{
		const auto curr_pos{ mobs.get<position>(p).pos };
		auto& next_pos{ mobs.get<next_position>(p).pos };
		const auto& coll{ mobs.get<collider>(p) };
		const auto [bbox_horizontal, bbox_vertical] = calc_next_bbox(curr_pos, next_pos, coll);

		if (has_collision(level, bbox_horizontal))
		{
			next_pos.x = curr_pos.x;
		}
		if (has_collision(level, bbox_vertical))
		{
			next_pos.y = curr_pos.y;
		}
	}
}

void aco::sys::submit_next_position(entt::registry& reg)
{
	auto view{ reg.view<position, next_position>() };
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

void aco::sys::center_view_on_player(entt::registry& reg, sf::View& render_view)
{
	auto players{ reg.view<player, position, animation>() };
	for (const auto p : players)
	{
		const auto curr_pos{ players.get<position>(p).pos };
		const auto center_offset{ players.get<animation>(p).frame_size / 2.0f };
		render_view.setCenter(curr_pos + center_offset);
	}
}
