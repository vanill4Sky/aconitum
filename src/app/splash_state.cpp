#include "splash_state.hpp"

#include <memory>

#include "constants.hpp"
#include "game_state.hpp"
#include "../util/graphics.hpp"

aco::splash_state::splash_state(aco::app_data& app_data)
	: m_app_data{ app_data }
{
}

void aco::splash_state::init()
{
	m_timer.restart();
	m_app_data.textures.load(logo_path);
	m_logo.setTexture(m_app_data.textures.get(logo_path));

	const sf::Vector2f desired_logo_size{ 200.0f, 200.0f };
	util::scale_sprite(m_logo, desired_logo_size);
	util::center_origin_of_sprite(m_logo);
	util::center_sprite(m_logo, m_app_data.window.getSize());
}

void aco::splash_state::handle_input()
{
	sf::Event event;
	while (m_app_data.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_app_data.window.close();
		}
	}
}

void aco::splash_state::update(float dt)
{
	if (m_timer.getElapsedTime().asSeconds() > aco::splash_screen_display_time)
	{
		m_app_data.state_manager.replace_state(std::make_unique<aco::game_state>(m_app_data));
	}
	m_logo.setScale(m_logo.getScale() * 1.01f);
}

void aco::splash_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(m_logo);
	m_app_data.window.display();
}
