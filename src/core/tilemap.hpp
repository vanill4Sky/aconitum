#pragma once

#include <cstdint>
#include <vector>

#include <SFML/Graphics.hpp>

namespace aco
{

struct tile
{
	tile()
		: is_blank{ true }
	{}
	tile(sf::Vector2f tex_coords)
		: tex_coords{ tex_coords }
	{}
	tile(float tex_coord_x, float tex_coord_y)
		: tex_coords{ tex_coord_x, tex_coord_y }
	{}

	sf::Vector2f tex_coords;
	bool is_blank{ false };
};

class tilemap : public sf::Drawable, public sf::Transformable
{	
public:
	tilemap();
	tilemap(const sf::Texture& tileset, sf::Vector2f tile_size, 
		const std::vector<aco::tile>& world_map, size_t width, size_t height);

private:
	void generate_vertex_array(sf::Vector2f tile_size,
		const std::vector<aco::tile>& world_map, size_t width, size_t height);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_tileset;
	sf::VertexArray m_vertices;
};

}