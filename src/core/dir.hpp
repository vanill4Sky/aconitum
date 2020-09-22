#pragma once

#include <SFML/System/Vector2.hpp>

namespace aco
{

enum class dir : int
{
	up,
	up_right,
	right,
	down_right,
	down,
	down_left,
	left,
	up_left,
	none,

	count = none
};

constexpr dir operator-(dir other)
{
	if (other == dir::none) 
		return dir::none;

	constexpr int dir_count{ static_cast<int>(dir::count) };
	int opposite_dir_value{ (static_cast<int>(other) - (dir_count / 2)) % (dir_count - 1) };

	return static_cast<dir>(opposite_dir_value);
}

constexpr dir& operator++(dir& other)
{
	if (other == dir::none)
		return other = dir::none;

	constexpr int dir_count{ static_cast<int>(dir::count) };
	int next_dir_value{ (static_cast<int>(other) + 1) % (dir_count - 1) };

	return other = static_cast<dir>(next_dir_value);
}

constexpr dir& operator--(dir& other)
{
	if (other == dir::none)
		return other = dir::none;

	constexpr int dir_count{ static_cast<int>(dir::count) };
	int next_dir_value{ (static_cast<int>(other) - 1) % (dir_count - 1) };

	return other = static_cast<dir>(next_dir_value);
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