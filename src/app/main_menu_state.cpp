#include "main_menu_state.hpp"

#include <memory>

#include "constants.hpp"
#include "game_state.hpp"
#include "../util/graphics.hpp"
#include "../app/editor_state.hpp"

aco::main_menu_state::main_menu_state(aco::app_data& app_data)
	: m_app_data{ app_data }
{
}

void aco::main_menu_state::init()
{
	m_app_data.textures.load(button_path);
	m_app_data.textures.load(manu_background_path);
	m_app_data.textures.load(menu_header_path);
	m_app_data.fonts.load(arial_path);

	m_background.setTexture(m_app_data.textures.get(manu_background_path));

	m_header = aco::button(
		m_app_data.textures.get(menu_header_path), 
		m_app_data.fonts.get(arial_path),  
		"", 30);
	m_start_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"New game", 30,
		[&] { m_app_data.state_manager.push_state(std::make_unique<aco::game_state>(m_app_data)); });
	m_continue_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Load game", 30);
	m_help_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Help", 30);
	m_about_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"About", 30);
	m_editor_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Level editor", 30,
		[&] { m_app_data.state_manager.push_state(std::make_unique<aco::editor_state>(m_app_data)); });
	m_exit_button = aco::button(
		m_app_data.textures.get(button_path),
		m_app_data.fonts.get(arial_path),
		"Exit game", 30,
		[&] { m_app_data.window.close(); });

	m_widgets.emplace_back(&m_header);
	m_widgets.emplace_back(&m_start_button);
	m_widgets.emplace_back(&m_continue_button);
	m_widgets.emplace_back(&m_help_button);
	m_widgets.emplace_back(&m_about_button);
	m_widgets.emplace_back(&m_editor_button);
	m_widgets.emplace_back(&m_exit_button);

	set_layout();
}

void aco::main_menu_state::handle_input()
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

void aco::main_menu_state::update(float dt)
{
}

void aco::main_menu_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(m_background);
	for (const auto* widget : m_widgets)
	{
		m_app_data.window.draw(*widget);
	}
	m_app_data.window.display();
}

void aco::main_menu_state::resume()
{
	set_layout();
}

void aco::main_menu_state::set_layout()
{
	sf::Vector2u size{ m_app_data.window.getSize() };
	auto view = util::resize_view(m_app_data.window.getView(), size);
	m_app_data.window.setView(view);

	constexpr float space{ 30.0f };
	float offset = 50.0f;
	for (auto* widget : m_widgets)
	{
		util::column_layout(*widget, size.x, offset);
		offset += (widget->size().y + space);
	}
}
