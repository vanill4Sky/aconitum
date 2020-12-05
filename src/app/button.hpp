#pragma once

#include <functional>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>

namespace aco
{

class button : public sf::Drawable, public sf::Transformable
{
public:
	button();
	button(const sf::Texture& texture, const sf::Font& font,
		const std::string& label, unsigned int character_size,
		const std::function<void()>& on_press_action);
	button(const sf::Texture& texture, const sf::Font& font, 
		const std::string& label, unsigned int character_size);

	void enable(bool is_active);
	sf::Vector2f size() const;
	void on_press_action(sf::Vector2f mouse_pos) const;

	static sf::Color enabled_label_color;
	static sf::Color disabled_label_color;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void center_label();

	sf::Sprite m_body;
	sf::Text m_label;
	bool m_is_active;
	std::function<void()> m_action;
};

}