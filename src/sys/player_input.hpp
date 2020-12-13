#pragma once

#include <entt/fwd.hpp>
#include <unordered_map>
#include <SFML/Window/Keyboard.hpp>

namespace aco::sys
{
/// <returns>Returns true if keyborad state has changed. Otherwise returns false.</returns>
bool key_pressed(entt::registry& reg, 
	std::unordered_map<sf::Keyboard::Key, bool>& keyboard_state, sf::Keyboard::Key key);
void key_released(entt::registry& reg,
	std::unordered_map<sf::Keyboard::Key, bool>& keyboard_state, sf::Keyboard::Key key);
}