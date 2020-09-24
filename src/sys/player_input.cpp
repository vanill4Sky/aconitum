#include "player_input.hpp"

#include "../core/dir.hpp"
#include "../comp/direction.hpp"
#include "../comp/player.hpp"
#include "../comp/entity_state.hpp"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

using namespace aco::comp;

namespace aco::sys
{
aco::dir combine(const aco::dir vertical, const aco::dir horizontal)
{
	switch (vertical)
	{
	case aco::dir::up:
		switch (horizontal)
		{
		case aco::dir::right:
			return aco::dir::up_right;
		case aco::dir::left:
			return aco::dir::up_left;
		default:
			return vertical;
		}
	case aco::dir::down:
		switch (horizontal)
		{
		case aco::dir::right:
			return aco::dir::down_right;
		case aco::dir::left:
			return aco::dir::down_left;
		default:
			return vertical;
		}
	default:
		return horizontal;
	}
}

aco::dir to_dir(std::unordered_map<sf::Keyboard::Key, bool>& keyboard_state, sf::Keyboard::Key key)
{
	aco::dir vertical{ aco::dir::none };
	if (keyboard_state[sf::Keyboard::W] || keyboard_state[sf::Keyboard::Up])
	{
		vertical = aco::dir::up;
	}
	else if (keyboard_state[sf::Keyboard::S] || keyboard_state[sf::Keyboard::Down])
	{
		vertical = aco::dir::down;
	}

	aco::dir horizontal{ aco::dir::none };
	if (keyboard_state[sf::Keyboard::A] || keyboard_state[sf::Keyboard::Left])
	{
		horizontal = aco::dir::left;
	}
	else if (keyboard_state[sf::Keyboard::D] || keyboard_state[sf::Keyboard::Right])
	{
		horizontal = aco::dir::right;
	}

	return combine(vertical, horizontal);
}

}

bool aco::sys::key_pressed(entt::registry& reg, 
	std::unordered_map<sf::Keyboard::Key, bool>& keyboard_state, sf::Keyboard::Key key)
{
	if (keyboard_state[key])
	{
		return false;
	}

	keyboard_state[key] = true;

	aco::dir dir{ to_dir(keyboard_state, key) };
	if (dir != aco::dir::none)
	{
		auto view = reg.view<player, direction>();
		for (const auto e : view)
		{
			view.get<direction>(e).dir = dir;
			if (!reg.has<move_state>(e))
			{
				reg.emplace<move_state>(e);
			}
		}
	}

	return true;
}

void aco::sys::key_released(entt::registry& reg, 
	std::unordered_map<sf::Keyboard::Key, bool>& keyboard_state, sf::Keyboard::Key key)
{
	keyboard_state[key] = false;

	aco::dir dir{ to_dir(keyboard_state, key) };
	auto view = reg.view<player, direction>();
	if (dir != aco::dir::none)
	{
		for (const auto e : view)
		{
			view.get<direction>(e).dir = dir;
			if (!reg.has<move_state>(e))
			{
				reg.emplace<move_state>(e);
			}
		}
	}
	else
	{
		for (const auto e : view)
		{
			reg.remove_if_exists<move_state>(e);
		}
	}
}
