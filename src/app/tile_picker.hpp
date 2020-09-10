#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

namespace aco
{

enum brush_mode
{
	bidirectional,
	horizontal_only,
	vertical_only,
	fixed
};

class tile_picker
{
public:
	tile_picker(const sf::Texture& tileset, float tile_size);

	sf::Sprite at(size_t pos_x, size_t pos_y) const;
	sf::Vector2<size_t> tile_count() const;
	size_t width() const;
	size_t height() const;

	void set_active_tile(size_t pos_x, size_t pos_y);
	void update_active_tile(int delta_x, int delta_y, brush_mode bm);
	sf::Vector2<size_t> active_tile() const;

private:
	sf::Vector2<size_t> calc_tile_count(const sf::Texture& tileset, unsigned int tile_size);
	std::vector<sf::Sprite> split_tileset(const sf::Texture& tileset, int tile_size);

	sf::Vector2<size_t> m_tile_count;
	std::vector<sf::Sprite> m_data;
	float m_tile_size;
	sf::Vector2<size_t> m_active_tile;
};

}