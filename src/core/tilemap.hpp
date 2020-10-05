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
	tile(sf::Vector2f tex_coords, bool is_collidable)
		: tile(tex_coords.x, tex_coords.y, is_collidable)
	{}
	tile(float tex_coord_x, float tex_coord_y, bool is_collidable)
		: tex_coords{ tex_coord_x, tex_coord_y }
		, is_collidable{ is_collidable }
	{}

	sf::Vector2f tex_coords;
	bool is_blank{ false };
	bool is_collidable{ false };
};

class tilemap : public sf::Drawable, public sf::Transformable
{	
public:
	tilemap();
	tilemap(const sf::Texture& tileset, sf::Vector2f tile_size, 
		const std::vector<aco::tile>& world_map, size_t width, size_t height, bool show_cooliders);

private:
	void generate_vertex_array(sf::Vector2f tile_size,
		const std::vector<aco::tile>& world_map, size_t width, size_t height, bool show_cooliders);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_tileset;
	sf::VertexArray m_vertices;
};

}