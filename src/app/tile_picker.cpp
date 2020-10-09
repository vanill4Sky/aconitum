#include "tile_picker.hpp"

#include <cassert>


aco::tile_picker::tile_picker()
	: m_tile_count{ 0, 0 }
	, m_data{ }
	, m_tile_size{ 1.0f }
	, m_active_tile{ 0, 0 }
{
}

aco::tile_picker::tile_picker(const sf::Texture& tileset, float tile_size)
	: m_tile_count{ calc_tile_count(tileset, static_cast<unsigned int>(tile_size)) }
	, m_data{ std::move(split_tileset(tileset, static_cast<int>(tile_size))) }
	, m_tile_size{ tile_size }
	, m_active_tile{ 0, 0 }
{ }

sf::Sprite aco::tile_picker::at(size_t pos_x, size_t pos_y) const
{
	assert(pos_x < m_tile_count.x);
	assert(pos_y < m_tile_count.y);

	return m_data[pos_y * m_tile_count.x + pos_x];
}

sf::Vector2<size_t> aco::tile_picker::tile_count() const
{
	return m_tile_count;
}

size_t aco::tile_picker::width() const
{
	return m_tile_count.x;
}

size_t aco::tile_picker::height() const
{
	return m_tile_count.y;
}

bool aco::tile_picker::is_tileset_loaded() const
{
	return m_tile_count != sf::Vector2<size_t>{ 0, 0 };
}

void aco::tile_picker::set_active_tile(size_t pos_x, size_t pos_y)
{
	m_active_tile = sf::Vector2<size_t>{
		pos_x % m_tile_count.x, pos_y % m_tile_count.y
	};
}

void aco::tile_picker::update_active_tile(int delta_x, int delta_y, aco::brush_mode bm)
{
	switch (bm)
	{
	case aco::bidirectional:
		break;
	case aco::horizontal_only:
		delta_y = 0;
		break;
	case aco::vertical_only:
		delta_x = 0;
		break;
	case aco::fixed:
	case aco::eraser:
		delta_x = 0;
		delta_y = 0;
		break;
	default:
		break;
	}

	m_active_tile = sf::Vector2<size_t>{
		(m_active_tile.x + delta_x) % m_tile_count.x,
		(m_active_tile.y + delta_y) % m_tile_count.y
	};
}

sf::Vector2<size_t> aco::tile_picker::active_tile() const
{
	return m_active_tile;
}

sf::Vector2<size_t> aco::tile_picker::calc_tile_count(const sf::Texture& tileset, unsigned int tile_size)
{
	return static_cast<sf::Vector2<size_t>>(tileset.getSize() / tile_size);
}

std::vector<sf::Sprite> aco::tile_picker::split_tileset(const sf::Texture& tileset, int tile_size)
{
	std::vector<sf::Sprite> tile_picker;

	for (size_t y = 0; y < m_tile_count.y; ++y)
	{
		for (size_t x = 0; x < m_tile_count.x; ++x)
		{
			tile_picker.emplace_back(
				tileset,
				sf::IntRect{
					static_cast<int>(x * tile_size), static_cast<int>(y * tile_size),
					tile_size, tile_size
				}
			);
		}
	}

	return tile_picker;
}