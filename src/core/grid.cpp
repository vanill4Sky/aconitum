#include "grid.hpp"

#include <spdlog/spdlog.h>

aco::grid::grid(sf::Vector2f tile_size, sf::Vector2f screen_size, sf::Color color)
	: m_tile_size{ tile_size }
	, m_color{ color }
	, m_is_visible{ true }
{
	fill_vertex_arrays(screen_size);
}

void aco::grid::resize(sf::Vector2f screen_size)
{
	fill_vertex_arrays(screen_size);
	spdlog::info("lines vertically: {}, lines horizontally: {}",
		m_vertical_lines.getVertexCount() / 2,
		m_horizontal_lines.getVertexCount() / 2);
}

void aco::grid::set_visible(bool is_visible)
{
	m_is_visible = is_visible;
}

void aco::grid::fill_vertex_arrays(sf::Vector2f screen_size)
{
	const sf::Vector2f lines_count{ 
		screen_size.x / m_tile_size.x + 1, screen_size.y / m_tile_size.y + 1 };
	const sf::Vector2<size_t> lines_count_u{ static_cast<sf::Vector2<size_t>>(lines_count) };

	m_vertical_lines.clear();
	m_vertical_lines.setPrimitiveType(sf::Lines);
	m_vertical_lines.resize(lines_count_u.x * 2);
	for (size_t i = 0; i < lines_count_u.x; ++i)
	{
		auto* current_line{ &m_vertical_lines[i * 2] };
		current_line[0].position = sf::Vector2f(m_tile_size.x * i, 0.0f);
		current_line[0].color = m_color;
		current_line[1].position = sf::Vector2f(m_tile_size.x * i, lines_count.y * m_tile_size.y);
		current_line[1].color = m_color;
	}

	m_horizontal_lines.clear();
	m_horizontal_lines.setPrimitiveType(sf::Lines);
	m_horizontal_lines.resize(lines_count_u.y * 2);
	for (size_t i = 0; i < lines_count_u.y; ++i)
	{
		auto* current_line{ &m_horizontal_lines[i * 2] };
		current_line[0].position = sf::Vector2f(0.0f, m_tile_size.y * i);
		current_line[0].color = m_color;
		current_line[1].position = sf::Vector2f(lines_count.x * m_tile_size.x, m_tile_size.y * i);
		current_line[1].color = m_color;
	}
}

void aco::grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_is_visible)
	{
		states.transform *= getTransform();
		target.draw(m_vertical_lines, states);
		target.draw(m_horizontal_lines, states);
	}
}
