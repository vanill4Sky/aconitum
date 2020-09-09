#pragma once

#include <SFML/Graphics.hpp>

namespace aco
{

class grid : public sf::Drawable, public sf::Transformable
{
public:
	grid(sf::Vector2f tile_size, sf::Vector2f screen_size, sf::Color color);

	void resize(sf::Vector2f screen_size);
	void set_visible(bool is_visible);

private:
	void fill_vertex_arrays(sf::Vector2f screen_size);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f m_tile_size;
	sf::Color m_color;
	sf::VertexArray m_vertical_lines;
	sf::VertexArray m_horizontal_lines;
	bool m_is_visible;
};

}
