#include "movement.hpp"

#include <entt/entt.hpp>

#include "../comp/direction.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/position.hpp"

using namespace aco::comp;

void aco::sys::movement(entt::registry& reg)
{
	auto view{ reg.view<position, velocity, move_state>() };
	for (const auto e : view)
	{
		const auto v{ view.get<velocity>(e) };
		view.get<position>(e).pos += aco::to_vec2<float>(v.dir) * v.speed;
	}
}
