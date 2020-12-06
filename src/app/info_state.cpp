#include "info_state.hpp"

#include <SFML/Window/Event.hpp>

#include "constants.hpp"
#include "../util/graphics.hpp"

aco::info_state::info_state(aco::app_data& app_data, std::string background_path)
	: m_app_data{ app_data }
	, m_background_path{ std::move(background_path) }
{
}

void aco::info_state::init()
{
	m_app_data.window.setView(m_app_data.window.getDefaultView());

	m_app_data.textures.load(button_path);
	m_app_data.textures.load(m_background_path);
	m_background.setTexture(m_app_data.textures.get(m_background_path));
	m_back_to_main_menu = aco::button{
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Go back", 30,
		[&] {m_app_data.state_manager.pop_state();} };

	m_widgets.emplace_back(&m_back_to_main_menu);

	set_layout();
}

void aco::info_state::handle_input()
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

void aco::info_state::update(float dt)
{
}

void aco::info_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(m_background);
	for (const auto* widget : m_widgets)
	{
		m_app_data.window.draw(*widget);
	}
	m_app_data.window.display();
}

void aco::info_state::set_layout()
{
	sf::Vector2u size{ m_app_data.window.getSize() };
	auto view = util::resize_view(m_app_data.window.getView(), size);
	m_app_data.window.setView(view);

	util::scale_sprite(m_background, static_cast<sf::Vector2f>(size));

	constexpr float space{ 30.0f };
	float offset = static_cast<float>(size.y);
	for (auto* widget : m_widgets)
	{
		offset -= (widget->size().y + space);
		util::column_layout(*widget, size.x, offset);
	}
}
