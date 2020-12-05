#pragma once

#include <cmath>

namespace util
{

template<typename VectorClass>
VectorClass normalize(const VectorClass& vec)
{
	auto length{ std::sqrt(vec.x * vec.x + vec.y * vec.y) };
	length = (length == 0.0f) ? 1.0f : length;

	return { vec.x / length, vec.y / length };
}

template<typename VectorClass>
float distance(const VectorClass& a, const VectorClass& b)
{
	return std::sqrtf(std::powf(b.x - a.x, 2) + std::powf(b.y - a.y, 2));
}

}