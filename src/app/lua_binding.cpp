#include "lua_binding.hpp"

#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <sol/sol.hpp>
#include <string>

#include "../comp/components.hpp"
#include "../core/dir.hpp"
#include "factories.hpp"
#include "modifiers.hpp"
#include "queries.hpp"

void aco::register_user_types(sol::state& state)
{
	using namespace aco::comp;

	state.new_usertype<entt::entity>("ex_entity");

	state.new_usertype<sf::Vector2f>("ex_vector2f",
		sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>(),
		"x", &sf::Vector2f::x,
		"y", &sf::Vector2f::y);

	state.new_enum("ex_dir",
		"up_left", dir::up_left, "left", dir::left,
		"down_left", dir::down_left, "up", dir::up, "none", dir::none,
		"down", dir::down, "up_right", dir::up_right, "right", dir::right,
		"down_right", dir::down_right, "count", dir::count);

	state.new_usertype<velocity>("ex_velocity",
		sol::constructors<velocity(), velocity(aco::dir, float)>(),
		"dir", &velocity::dir,
		"speed", &velocity::speed);
	state.new_usertype<position>("ex_position",
		sol::constructors<position(), position(sf::Vector2f), position(float, float)>(),
		"pos", &position::pos);
	state.new_usertype<next_position>("ex_next_position",
		sol::constructors<next_position(), next_position(sf::Vector2f), next_position(float, float)>(),
		"pos", &next_position::pos);
	state.new_usertype<animation>("ex_animation",
		sol::constructors<animation(), animation(sf::Vector2f, int), animation(float, float, int)>(),
		"frame_size", &animation::frame_size,
		"frame_count", &animation::frame_count);
	state.new_usertype<target>("ex_target",
		sol::constructors<target(), target(float, float)>(),
		"entity", &target::entity,
		"trigger_distance", &target::trigger_distance,
		"stop_distance", &target::stop_distance);
	state.new_usertype<collider>("ex_collider",
		sol::constructors<collider(), collider(sf::Vector2f, sf::Vector2f), collider(float, float, float, float)>(),
		"offset", &collider::offset,
		"size", &collider::size);
	state.new_usertype<kinematic>("ex_kinematic");
	state.new_usertype<trigger>("ex_trigger");
	state.new_usertype<sprite>("ex_sprite",
		sol::constructors<sprite(), sprite(std::string), sprite(std::string, bool)>(),
		"tex_path", &sprite::tex_path,
		"is_ground_object", &sprite::is_ground_object);
	state.new_usertype<iob>("ex_iob");
	state.new_usertype<box>("ex_box");
	state.new_usertype<door>("ex_door");
	state.new_usertype<pressure_plate>("ex_pressure_plate");
	state.new_usertype<torch>("ex_torch");
	state.new_usertype<lever_switch>("ex_lever_switch",
		sol::constructors<lever_switch(), lever_switch(float)>(),
		"activation_distance", &lever_switch::activation_distance);
	state.new_usertype<player>("ex_player");
	state.new_usertype<mob>("ex_mob");
	state.new_usertype<stalker>("ex_stalker");
	state.new_usertype<idle_state>("ex_idle_state");
}

void aco::register_factory(sol::state& state, entt::registry& reg, resource_holder<sf::Texture>& textures)
{
	state.set_function("ex_create_entity",
		[&](const sol::table& entity_template, sf::Vector2f position, size_t idx) {
			return create_entity(reg, textures, entity_template, position, idx);
		});
}

void aco::register_modifiers(sol::state& state, entt::registry& reg)
{
	state.set_function("ex_set_target",
		[&](entt::entity self, entt::entity target) {
			return set_target(reg, self, target);
		});

	state.set_function("ex_connect_torch",
		[&](entt::entity self, entt::entity torch) {
			return connect_torch(reg, self, torch);
		});
}

void aco::register_queries(sol::state& state, entt::registry& reg)
{
	state.set_function("ex_has_active_state",
		[&](entt::entity e) {
			return has_active_state(reg, e);
		});
}
