#include "grid.hpp"

aco::grid::grid(sf::Vector2f tile_size, sf::Vector2f screen_size, sf::Color color)
	: m_tile_size{ tile_size }
	, m_color{ color }
{
	generate_vertex_array(screen_size);
}

void aco::grid::resize(sf::Vector2f screen_size)
{
}

void aco::grid::generate_vertex_array(sf::Vector2f screen_size)
{
	const sf::Vector2f lines_count{ 
		screen_size.x / m_tile_size.x + 1, screen_size.y / m_tile_size.y + 1 };
	const sf::Vector2u lines_count_u{ static_cast<sf::Vector2u>(lines_count) };
	const auto vertex_count{ (lines_count_u.x + lines_count_u.y) * 2 };

	m_vertices.setPrimitiveType(sf::Lines);
	m_vertices.resize(vertex_count);

	for (size_t i = 0; i < lines_count_u.x; ++i)
	{
		auto* current_line{ &m_vertices[i * 2] };
		current_line[0].position = sf::Vector2f(m_tile_size.x * i, 0.0f);
		current_line[0].color = m_color;
		current_line[1].position = sf::Vector2f(m_tile_size.x * i, lines_count.y * m_tile_size.y);
		current_line[1].color = m_color;
	}

	for (size_t i = 0; i < lines_count_u.y; ++i)
	{
		auto* current_line{ &m_vertices[(i + lines_count_u.x) * 2] };
		current_line[0].position = sf::Vector2f(0.0f, m_tile_size.y * i);
		current_line[0].color = m_color;
		current_line[1].position = sf::Vector2f(lines_count.x * m_tile_size.x, m_tile_size.y * i);
		current_line[1].color = m_color;
	}
}

void aco::grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_vertices, states);
}
