#include "game_state.hpp"

#include <SFML/Window/Event.hpp>
#include <cassert>

#include "sprite_picker.hpp"
#include "../sys/player_input.hpp"
#include "../sys/movement.hpp"
#include "../sys/render.hpp"
#include "../sys/target_following.hpp"
#include "factories.hpp"

aco::game_state::game_state(aco::app_data& app_data)
	: m_app_data{ app_data }
{
}

void aco::game_state::init()
{
	player_tex.loadFromFile("assets/textures/player_thief_01.png");
	stalker_tex.loadFromFile("assets/textures/stalker_thief_01.png");
	box_tex.loadFromFile("assets/textures/box_01.png");

	const auto player_entity{ aco::create_player(m_reg, player_tex) };
	aco::create_stalker(m_reg, player_entity, stalker_tex);
	sf::Vector2f box_pos{ 600.0f, 0.0f };
	for (int i = 0; i < 10; ++i)
	{
		aco::create_box(m_reg, box_tex, box_pos);
		aco::create_box(m_reg, box_tex, box_pos + sf::Vector2f{ 100.0f, 0.0f });
		box_pos.y += 100.0f;
	}
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
	aco::sys::localize_target(m_reg);
	aco::sys::target_triggers(m_reg);
	aco::sys::find_next_position(m_reg);
	aco::sys::player_iob_collide(m_reg);
	aco::sys::submit_next_position(m_reg);
}

void aco::game_state::draw()
{
	m_app_data.window.clear();

	aco::sys::draw_entities(m_reg, m_app_data.window, frame_cnt);

	m_app_data.window.display();

	++frame_cnt;
}
