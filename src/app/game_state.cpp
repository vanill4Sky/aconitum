#include "game_state.hpp"

#include <SFML/Window/Event.hpp>

#include "sprite_picker.hpp"
#include "../sys/player_input.hpp"
#include "../sys/movement.hpp"
#include "../sys/render.hpp"
#include "../comp/direction.hpp"
#include "../comp/player.hpp"
#include "../comp/entity_state.hpp"
#include "../comp/position.hpp"
#include "../comp/sprite.hpp"

using namespace aco::comp;

aco::game_state::game_state(aco::app_data& app_data)
	: m_app_data{ app_data }
{
}

void aco::game_state::init()
{
	player_tex.loadFromFile("assets/textures/player_thief_01.png");
	player_sprite.setTexture(player_tex);
	player_sprite.setTextureRect(aco::pick_sprite(32.0f, aco::pose::walkcycle, aco::dir::down_left, 0));

	const auto e{ m_reg.create() };
	m_reg.emplace<aco::comp::player>(e);
	m_reg.emplace<aco::comp::direction>(e);
	m_reg.emplace<aco::comp::position>(e, sf::Vector2f{ 500.0f, 500.0f });
	m_reg.emplace<aco::comp::sprite>(e, player_sprite);
}

void aco::game_state::handle_input()
{
	sf::Event event;
	while (m_app_data.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_app_data.window.close();
		} 
		else if (event.type == sf::Event::KeyPressed)
		{
			aco::sys::key_pressed(m_reg, m_keyboard_state, event.key.code);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			aco::sys::key_released(m_reg, m_keyboard_state, event.key.code);
		}
	}
}

void aco::game_state::update(float dt)
{
	aco::sys::movement(m_reg);
}

void aco::game_state::draw()
{
	m_app_data.window.clear();

	aco::sys::draw_player(m_reg, m_app_data.window, frame_cnt);

	m_app_data.window.display();

	++frame_cnt;
}
