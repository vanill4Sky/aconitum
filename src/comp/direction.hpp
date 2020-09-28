#pragma once

#include "../core/dir.hpp"

namespace aco::comp
{

struct velocity
{
	aco::dir dir{ aco::dir::down };
	float speed;
};

}