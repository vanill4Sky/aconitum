#pragma once

#include <entt/entt.hpp>

namespace aco::comp
{

struct target
{
	target(entt::entity enttity, float trigger_distance, float stop_distance)
		: entity{ enttity }, trigger_distance{ trigger_distance }, stop_distance{ stop_distance } {}
	target(float trigger_distance, float stop_distance)
		: target(entt::null, trigger_distance, stop_distance) {}
	target() 
		: target(entt::null, 0.0f, 0.0f) {}

	entt::entity entity;
	float trigger_distance;
	float stop_distance;
};

}