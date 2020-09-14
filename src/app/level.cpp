#include "level.hpp"

#include <spdlog/spdlog.h>

aco::level::level(sf::Texture tileset, float tile_size, size_t width, size_t height)
	: m_tileset{ std::move(tileset) }
	, m_tile_size{ tile_size }
	, m_width{ width }
	, m_height{ height }
{
	m_data.resize(width * height);

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			m_data.at(y * width + x) = aco::tile(32.0f * x, 32.0f * y);
		}
	}
}

aco::tile& aco::level::at(int pos_x, int pos_y)
{
	size_t abs_pos_x{ static_cast<size_t>(std::abs(pos_x)) };
	size_t abs_pos_y{ static_cast<size_t>(std::abs(pos_y)) };

	if (pos_x < 0 && pos_y < 0)
	{
		resize(m_width + abs_pos_x, m_height + abs_pos_y, abs_pos_x, abs_pos_y);
		abs_pos_x = 0;
		abs_pos_y = 0;
		move({ static_cast<float>(pos_x), static_cast<float>(pos_y) });
	}
	else if (pos_x < 0)
	{
		resize(m_width + abs_pos_x, m_height, abs_pos_x, 0);
		abs_pos_x = 0;
		move({ static_cast<float>(pos_x), 0.0f });
	}
	else if (pos_y < 0)
	{
		resize(m_width, m_height + abs_pos_y, 0, abs_pos_y);
		abs_pos_y = 0;
		move({ 0.0f, static_cast<float>(pos_y) });
	}

	if (abs_pos_x >= m_width || abs_pos_y >= m_height)
	{
		resize(std::max(abs_pos_x + 1, m_width), std::max(abs_pos_y + 1, m_height));
	}

	return m_data[abs_pos_y * m_width + abs_pos_x];
}

void aco::level::update_tilemap()
{
	m_tilemap = aco::tilemap(m_tileset, { m_tile_size, m_tile_size }, m_data, m_width, m_height);
}

const aco::tilemap& aco::level::get_tilemap() const
{
	return m_tilemap;
}

const sf::RenderStates& aco::level::level_render_states() const
{
	return m_level_render_states;
}

const sf::Vector2i aco::level::render_translation() const
{
	auto matrix{ m_level_render_states.transform.getMatrix() };
	return static_cast<sf::Vector2i>((sf::Vector2f{ matrix[12], matrix[13] } / m_tile_size));
}

float aco::level::tile_size() const
{
	return m_tile_size;
}

void aco::level::move(sf::Vector2f delta)
{
	m_level_render_states.transform.translate(m_tile_size * delta);
}

void aco::level::resize(size_t new_width, size_t new_height,
	size_t offset_x, size_t offset_y)
{
	std::vector<aco::tile> temp{ new_width * new_height };
	auto horizontal_limit{ std::min(m_width, new_width) };
	auto vertical_limit{ std::min(m_height, new_height) };

	for (size_t y{ 0 }; y < vertical_limit; ++y)
	{
		for (size_t x{ 0 }; x < horizontal_limit; ++x)
		{
			temp[(y + offset_y) * new_width + x + offset_x] = m_data[y * m_width + x];
		}
	}

	m_data = std::move(temp);
	m_width = new_width;
	m_height = new_height;

	spdlog::info("current world size in tiles ({}, {})", m_width, m_height);
}
