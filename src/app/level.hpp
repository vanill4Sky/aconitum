#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

#include "../core/tilemap.hpp"

namespace aco
{

class level
{
public:
	level(sf::Texture tileset, float tile_size, size_t width, size_t height);

	aco::tile& at(size_t pos_x, size_t pos_y);
	void update_tilemap();
	const aco::tilemap& get_tilemap() const;
	float tile_size() const;

private:
	void resize(size_t new_width, size_t new_height);

	sf::Texture m_tileset;
	aco::tilemap m_tilemap;
	float m_tile_size;
	std::vector<aco::tile> m_data;
	size_t m_width;
	size_t m_height;
};

}