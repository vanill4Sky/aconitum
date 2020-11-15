#include "lua_binding.hpp"

#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <sol/sol.hpp>
#include <string>

#include "../comp/components.hpp"

void aco::register_user_types(sol::state& state)
{
	using namespace aco::comp;

	state.new_usertype<entt::entity>("ex_entity");

	state.new_usertype<sf::Vector2f>("ex_vector2f",
		sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>(),
		"x", &sf::Vector2f::x,
		"y", &sf::Vector2f::y);

	state.new_usertype<position>("ex_position",
		sol::constructors<position(), position(sf::Vector2f), position(float, float)>(),
		"pos", &position::pos);
	state.new_usertype<next_position>("ex_next_position",
		sol::constructors<next_position(), next_position(sf::Vector2f), next_position(float, float)>(),
		"pos", &next_position::pos);
	state.new_usertype<animation>("ex_animation",
		sol::constructors<animation(), animation(sf::Vector2f), animation(float, float)>(),
		"frame_size", &animation::frame_size);
	state.new_usertype<target>("ex_target",
		sol::constructors<target(), target(float, float)>(),
		"entity", &target::entity,
		"trigger_distance", &target::trigger_distance,
		"stop_distance", &target::stop_distance);
	state.new_usertype<collider>("ex_collider",
		sol::constructors<collider(), collider(sf::Vector2f, sf::Vector2f), collider(float, float, float, float)>(),
		"offset", &collider::offset,
		"size", &collider::size);
	state.new_usertype<sprite>("ex_sprite",
		sol::constructors<sprite(), sprite(std::string)>(),
		"tex_path", &sprite::tex_path);
	state.new_usertype<iob>("ex_iob");
	state.new_usertype<box>("ex_box");
	state.new_usertype<move_state>("ex_move_state");
	state.new_usertype<player>("ex_player");
	state.new_usertype<mob>("ex_mob");
	state.new_usertype<stalker>("ex_stalker");
}