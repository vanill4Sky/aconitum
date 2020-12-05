#pragma once

#include <entt/fwd.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace aco::sys
{

void toggle_lever(entt::registry& reg, sf::Keyboard::Key key_code);
void activate_lever(entt::registry& reg);

}