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

}