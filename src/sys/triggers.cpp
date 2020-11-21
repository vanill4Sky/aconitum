#include "triggers.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "../comp/entity_state.hpp"
#include "../comp/iob.hpp"
#include "../comp/collider.hpp"
#include "../comp/position.hpp"

using namespace aco::comp;

void aco::sys::update_pressure_plates(entt::registry& reg)
{
	auto plates{ reg.view<pressure_plate, position, collider>() };
	auto iobs{ reg.view<iob, position, collider>(entt::exclude<pressure_plate>) };

	for (const auto i : iobs)
	{
		const auto i_pos{ iobs.get<position>(i).pos };
		const auto& i_collider{ iobs.get<collider>(i) };
		sf::FloatRect i_bbox{ i_pos + i_collider.offset, i_collider.size };

		for (const auto p : plates)
		{
			const auto p_pos{ plates.get<position>(p).pos };
			const auto& p_collider{ plates.get<collider>(p) };
			sf::FloatRect p_bbox{ p_pos + p_collider.offset, p_collider.size };

			if (i_bbox.intersects(p_bbox))
			{
				if (reg.has<trigger>(p))
				{
					reg.emplace_or_replace<active_state>(p);
				}
			}
		}
	}
}

void aco::sys::clear_triggers(entt::registry& reg)
{
	const auto view{ reg.view<active_state>() };

	for (const auto e : view)
	{
		reg.remove<active_state>(e);
	}
}
