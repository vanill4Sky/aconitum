#pragma once

#include <cstdint>
#include <vector>

#include <SFML/Graphics.hpp>

namespace aco
{

struct tile
{
	tile(sf::Vector2u tex_coords)
		: tex_coords{ tex_coords }
	{}

	sf::Vector2u tex_coords;
};

class tilemap : public sf::Drawable, public sf::Transformable
{	
public:
	tilemap(const sf::Texture& tileset, sf::Vector2u tile_size, 
		const std::vector<aco::tile>& world_map, size_t width, size_t height);

private:
	void generate_vertex_array(sf::Vector2u tile_size,
		const std::vector<aco::tile>& world_map, size_t width, size_t height);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_tileset;
	sf::VertexArray m_vertices;
};

}