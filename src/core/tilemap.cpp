#include "tilemap.hpp"

aco::tilemap::tilemap(const sf::Texture& tileset, sf::Vector2f tile_size, 
	const std::vector<aco::tile>& world_map, size_t width, size_t height)
	: m_tileset{ tileset }
{
	generate_vertex_array(tile_size, world_map, width, height);
}

void aco::tilemap::generate_vertex_array(sf::Vector2f tile_size, 
	const std::vector<aco::tile>& world_map, size_t width, size_t height)
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	for (size_t i{ 0 }; i < width; ++i)
	{
		for (size_t j{ 0 }; j < height; ++j)
		{
			const auto current_tile_idx{ i + j * width };

			auto* current_tile_quad{ &m_vertices[current_tile_idx * 4] };

			current_tile_quad[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
			current_tile_quad[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
			current_tile_quad[2].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
			current_tile_quad[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);

			const auto curent_tile_tex{ world_map[current_tile_idx].tex_coords };
			current_tile_quad[0].texCoords = sf::Vector2f(curent_tile_tex.x, curent_tile_tex.y);
			current_tile_quad[1].texCoords = sf::Vector2f(curent_tile_tex.x + tile_size.x, curent_tile_tex.y);
			current_tile_quad[2].texCoords = sf::Vector2f(curent_tile_tex.x + tile_size.x, curent_tile_tex.y + tile_size.y);
			current_tile_quad[3].texCoords = sf::Vector2f(curent_tile_tex.x, curent_tile_tex.y + tile_size.y);
		}
	}
}

void aco::tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}
