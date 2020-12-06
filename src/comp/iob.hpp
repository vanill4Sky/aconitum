#pragma once

#include <entt/fwd.hpp>
#include <vector>

namespace aco::comp
{

struct iob {};
struct box {};
struct pressure_plate {};
struct door {};
struct torch {};
struct lever_switch 
{
	lever_switch(float activation_distance)
		: activation_distance{ activation_distance }
	{}
	lever_switch()
		: lever_switch(10.0f)
	{}

	float activation_distance;
	std::vector<entt::entity> torches;
};

}