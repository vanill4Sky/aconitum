#pragma once

#include <array>
#include <cassert>

#include <SFML/System/Vector2.hpp>

namespace aco
{

enum class dir : int
{
	up_left,
	left,
	down_left,
	up,
	none,
	down,
	up_right,
	right,
	down_right,

	count
};

static const std::array<dir, static_cast<size_t>(dir::count)> dir_to_vec2_map{
	dir::up_left, dir::left, dir::down_left, dir::up,
	dir::none, dir::down, dir::up_right, dir::right, dir::down_right
};

template<typename T>
constexpr dir to_dir(sf::Vector2<T> vec)
{
	const auto ternary_num{ static_cast<sf::Vector2<int>>(vec) + sf::Vector2i{1, 1} };
	const auto idx{ ternary_num.x * 3 + ternary_num.y };
	assert(idx >= 0 && idx < static_cast<size_t>(dir::count));

	return dir_to_vec2_map[idx];
}

template<typename T> 
constexpr sf::Vector2<T> to_vec2(aco::dir dir)
{
	switch (dir)
	{
	case aco::dir::up:
		return { static_cast<T>(0), static_cast<T>(-1) };
	case aco::dir::up_right:
		return { static_cast<T>(1), static_cast<T>(-1) };
	case aco::dir::right:
		return { static_cast<T>(1), static_cast<T>(0) };
	case aco::dir::down_right:
		return { static_cast<T>(1), static_cast<T>(1) };
	case aco::dir::down:
		return { static_cast<T>(0), static_cast<T>(1) };
	case aco::dir::down_left:
		return { static_cast<T>(-1), static_cast<T>(1) };
	case aco::dir::left:
		return { static_cast<T>(-1), static_cast<T>(0) };
	case aco::dir::up_left:
		return { static_cast<T>(-1), static_cast<T>(-1) };
	default:
		return { static_cast<T>(0), static_cast<T>(0) };
		break;
	}
}

}