#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "../core/tilemap.hpp"

namespace aco
{

class level
{
public:
	level(sf::Texture tileset, float tile_size, size_t width, size_t height);

	aco::tile& at(int pos_x, int pos_y);
	void move(sf::Vector2f delta);
	void update_tilemap();

	const aco::tilemap& get_tilemap() const;
	const sf::RenderStates& level_render_states() const;
	const sf::Vector2i render_translation() const;
	float tile_size() const;


private:
	void resize(size_t new_width, size_t new_height, 
		size_t offset_x = 0, size_t offset_y = 0);

	sf::Texture m_tileset;
	aco::tilemap m_tilemap;
	sf::RenderStates m_level_render_states;
	float m_tile_size;
	std::vector<aco::tile> m_data;
	size_t m_width;
	size_t m_height;
};

}