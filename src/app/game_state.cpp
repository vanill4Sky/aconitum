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

	const auto player_entity{ aco::create_player(m_reg, player_tex) };
	aco::create_stalker(m_reg, stalker_tex, player_entity);

	sf::Vector2i vec{ -1, 0 };
	assert(aco::to_vec2<int>(aco::to_dir(vec)) == vec);
	vec = sf::Vector2i{ -1, -1 };
	assert(aco::to_vec2<int>(aco::to_dir(vec)) == vec);
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
	aco::sys::movement(m_reg);
}

void aco::game_state::draw()
{
	m_app_data.window.clear();

	aco::sys::draw_player(m_reg, m_app_data.window, frame_cnt);
	aco::sys::draw_creatures(m_reg, m_app_data.window, frame_cnt);

	m_app_data.window.display();

	++frame_cnt;
}
