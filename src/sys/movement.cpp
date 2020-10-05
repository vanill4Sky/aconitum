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

#include <spdlog/spdlog.h>

using namespace aco::comp;

namespace aco::sys
{

sf::Vector2<bool> iob_iob_collide(entt::registry& reg, entt::entity e)
{
	auto iobs{ reg.view<iob, position, next_position, collider>() };
	assert(iobs.contains(e));

	const auto e_curr_pos{ iobs.get<position>(e).pos };
	auto& e_next_pos{ iobs.get<next_position>(e).pos };
	auto delta{ e_curr_pos - e_next_pos };
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
			if (delta.x != 0.0f)
			{
				return sf::Vector2<bool>{ true, false };
			}
			else if (delta.y != 0.0f)
			{
				return sf::Vector2<bool>{ false, true };
			}
		}
	}

	return sf::Vector2<bool>{ false, false };
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

void aco::sys::player_iob_collide(entt::registry& reg)
{
	auto players{ reg.view<player, position, next_position, collider, velocity>() };
	auto iobs{ reg.view<iob, position, next_position, collider>() };

	for (const auto p : players)
	{
		const auto p_curr_pos{ players.get<position>(p).pos };
		auto& p_next_pos{ players.get<next_position>(p).pos };
		const auto& p_collider{ players.get<collider>(p) };
		sf::FloatRect p_bbox{ p_next_pos + p_collider.offset, p_collider.size };

		for (const auto i : iobs)
		{
			const auto i_curr_pos{ iobs.get<position>(i).pos };
			auto& i_next_pos{ iobs.get<next_position>(i).pos };
			const auto& i_collider{ iobs.get<collider>(i) };
			sf::FloatRect i_bbox{ i_curr_pos + i_collider.offset, i_collider.size };

			if (sf::FloatRect intersection;
				p_bbox.intersects(i_bbox, intersection))
			{

				auto vel{ players.get<velocity>(p) };
				auto dir_vec{ aco::to_vec2<float>(vel.dir) };

				if (intersection.height > intersection.width)
				{
					dir_vec.y = 0;
				}
				else if (intersection.height < intersection.width)
				{
					dir_vec.x = 0;
				}
				else
				{
					dir_vec.x = 0;
					dir_vec.y = 0;
				}

				i_next_pos += sf::Vector2f{ intersection.width * dir_vec.x, intersection.height * dir_vec.y };

				auto pushed_iob_collision{ iob_iob_collide(reg, i) };
				if (pushed_iob_collision.x)
				{
					i_next_pos.x = i_curr_pos.x;
					p_next_pos.x = p_curr_pos.x;
				}
				else if (pushed_iob_collision.y)
				{
					i_next_pos.y = i_curr_pos.y;
					p_next_pos.y = p_curr_pos.y;
				}
			}
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
