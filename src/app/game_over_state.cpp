#include "game_over_state.hpp"

#include <SFML/Window/Event.hpp>

#include "constants.hpp"
#include "../util/graphics.hpp"
#include "game_state.hpp"

aco::game_over_state::game_over_state(aco::app_data& app_data, bool has_win)
	: m_app_data{ app_data }
	, m_has_win{ has_win }
{
}

void aco::game_over_state::init()
{
	m_app_data.window.setView(m_app_data.window.getDefaultView());

	m_app_data.textures.load(button_path);
	m_retry = aco::button{
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Try again", 30,
		[&] { m_app_data.state_manager.replace_state(std::make_unique<game_state>(m_app_data)); } };
	m_back_to_main_menu = aco::button{
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Main menu", 30,
		[&] {
			m_app_data.state_manager.pop_state(); 
	}};

	if (m_has_win)
	{
		m_app_data.textures.load(game_win_background);
		m_background.setTexture(m_app_data.textures.get(game_win_background));
	}
	else
	{
		m_app_data.textures.load(game_lost_background);
		m_background.setTexture(m_app_data.textures.get(game_lost_background));
		m_widgets.emplace_back(&m_retry);
	}

	m_widgets.emplace_back(&m_back_to_main_menu);

	set_layout();
}

void aco::game_over_state::handle_input()
{
	sf::Event event;
	while (m_app_data.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_app_data.window.close();
		}
		else if (event.type == sf::Event::Resized)
		{
			set_layout();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_app_data.state_manager.pop_state();
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2i mouse_pos{ event.mouseButton.x, event.mouseButton.y };
			for (auto* widget : m_widgets)
			{
				widget->on_press_action(static_cast<sf::Vector2f>(mouse_pos));
			}
		}
	}
}

void aco::game_over_state::update(float dt)
{
}

void aco::game_over_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(m_background);
	for (const auto* widget : m_widgets)
	{
		m_app_data.window.draw(*widget);
	}
	m_app_data.window.display();
}

void aco::game_over_state::set_layout()
{
	sf::Vector2u size{ m_app_data.window.getSize() };
	auto view = util::resize_view(m_app_data.window.getView(), size);
	m_app_data.window.setView(view);

	util::center_origin_of_sprite(m_background);
	util::center_sprite(m_background, size);

	constexpr float space{ 30.0f };
	auto offset = m_background.getPosition().y - 2 * space;
	for (auto* widget : m_widgets)
	{
		util::column_layout(*widget, size.x, offset);
		offset += (widget->size().y + space);
	}
}
