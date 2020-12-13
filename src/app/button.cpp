#include "button.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "../util/graphics.hpp"
#include <spdlog/spdlog.h>

sf::Color aco::button::enabled_label_color = sf::Color::White;
sf::Color aco::button::disabled_label_color = sf::Color(125, 125, 125);

aco::button::button()
	: m_is_active{ true }
{
}

aco::button::button(const sf::Texture& texture, const sf::Font& font, 
	const std::string& label, unsigned int character_size, 
	const std::function<void()>& on_press_action)
	: m_body{ texture }
	, m_label{ label, font, character_size }
	, m_is_active{ true }
	, m_action{ on_press_action }
{
	center_label();
}

aco::button::button(const sf::Texture& texture, const sf::Font& font,
	const std::string& label, unsigned int character_size)
	: button(texture, font, label, character_size, []{})
{
}


void aco::button::enable(bool is_active)
{
	m_is_active = is_active;
	if (m_is_active)
	{
		m_label.setFillColor(enabled_label_color);
	}
	else
	{
		m_label.setFillColor(disabled_label_color);
	}
}

sf::Vector2f aco::button::size() const
{
	const auto gb = m_body.getGlobalBounds();

	return { gb.width, gb.height };
}

void aco::button::on_press_action(sf::Vector2f mouse_pos) const
{
	if (m_is_active)
	{
		if (const sf::FloatRect bounds{getPosition(), size()};
			bounds.contains(mouse_pos))
		{
			m_action();
		}
	}
}

void aco::button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_body, states);
	target.draw(m_label, states);
}

void aco::button::center_label()
{
	util::center_origin(m_label);
	const sf::Vector2f body_size{ 
		m_body.getGlobalBounds().width, 
		m_body.getGlobalBounds().height };
	m_label.move(body_size / 2.0f);
}
