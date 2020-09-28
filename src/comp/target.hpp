#pragma once

#include <entt/fwd.hpp>

namespace aco::comp
{

struct target
{
	entt::entity entity;
	float trigger_distance;
	float stop_distance;
};

}