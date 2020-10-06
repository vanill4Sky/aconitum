#pragma once

#include <cstdint>
#include <vector>
#include <variant>

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
	tilemap(std::variant<sf::Color, sf::Texture> tile_fill, sf::Vector2f tile_size, 
		const std::vector<aco::tile>& world_map, size_t width, size_t height);

private:
	void generate_textured_tilemap(sf::Vector2f tile_size,
		const std::vector<aco::tile>& world_map, size_t width, size_t height);
	void generate_colored_tilemap(sf::Vector2f tile_size,
		const std::vector<aco::tile>& world_map, size_t width, size_t height);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::variant<sf::Color, sf::Texture> m_tile_fill;
	sf::VertexArray m_vertices;
};

}