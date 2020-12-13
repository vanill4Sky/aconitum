#pragma once

#include "../core/dir.hpp"

namespace aco::comp
{

struct velocity
{
	velocity(aco::dir dir, float speed)
		: dir{ dir }, speed{ speed } {}
	velocity() 
		: velocity(aco::dir::down, 0.0f) {}

	aco::dir dir;
	float speed;
};

}