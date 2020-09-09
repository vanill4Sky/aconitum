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

aco::tile& aco::level::at(size_t pos_x, size_t pos_y)
{
	if (pos_x >= m_width || pos_y >= m_height)
	{
		resize(std::max(pos_x + 1, m_width), std::max(pos_y + 1, m_height));
	}

	return m_data[pos_y * m_width + pos_x];
}

aco::tilemap aco::level::get_tilemap() const
{
	return aco::tilemap(m_tileset, {m_tile_size, m_tile_size}, m_data, m_width, m_height);
}

void aco::level::resize(size_t new_width, size_t new_height)
{
	std::vector<aco::tile> temp{ new_width * new_height };
	auto horizontal_limit{ std::min(m_width, new_width) };
	auto vertical_limit{ std::min(m_height, new_height) };

	for (size_t y{ 0 }; y < vertical_limit; ++y)
	{
		for (size_t x{ 0 }; x < horizontal_limit; ++x)
		{
			temp[y * new_width + x] = m_data[y * m_width + x];
		}
	}

	m_data = std::move(temp);
	m_width = new_width;
	m_height = new_height;

	spdlog::info("current world size in tiles ({}, {})", m_width, m_height);
}
