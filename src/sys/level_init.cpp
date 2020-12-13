#include "level_init.hpp"

#include <entt/entt.hpp>

using namespace aco::comp;

void aco::sys::fill_orderd_sprites(entt::registry& reg, std::vector<const sprite*>& ordered_sprites)
{
	ordered_sprites.clear();

	const auto view{ reg.view<sprite>() };
	for (const auto e : view)
	{
		const sprite* s{ &view.get<sprite>(e) };
		ordered_sprites.emplace_back(s);
	}
}
