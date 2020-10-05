#include "tilemap.hpp"

#include <algorithm>

aco::tilemap::tilemap() {}

aco::tilemap::tilemap(const sf::Texture& tileset, sf::Vector2f tile_size,
	const std::vector<aco::tile>& world_map, size_t width, size_t height, bool show_cooliders)
	: m_tileset{ tileset }
{
	generate_vertex_array(tile_size, world_map, width, height, show_cooliders);
}

void aco::tilemap::generate_vertex_array(sf::Vector2f tile_size, 
	const std::vector<aco::tile>& world_map, size_t width, size_t height, bool show_cooliders)
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	for (size_t i{ 0 }; i < height; ++i)
	{
		for (size_t j{ 0 }; j < width; ++j)
		{
			const auto current_tile_idx{ i * width + j};

			if (world_map[current_tile_idx].is_blank)
			{
				continue;
			}

			auto* current_tile_quad{ &m_vertices[current_tile_idx * 4] };
			current_tile_quad[0].position = sf::Vector2f(j * tile_size.x, i * tile_size.y);
			current_tile_quad[1].position = sf::Vector2f((j + 1) * tile_size.x, i * tile_size.y);
			current_tile_quad[2].position = sf::Vector2f((j + 1) * tile_size.x, (i + 1) * tile_size.y);
			current_tile_quad[3].position = sf::Vector2f(j * tile_size.x, (i + 1) * tile_size.y);

			const auto curent_tile_tex{ world_map[current_tile_idx].tex_coords };
			current_tile_quad[0].texCoords = sf::Vector2f(curent_tile_tex.x, curent_tile_tex.y);
			current_tile_quad[1].texCoords = sf::Vector2f(curent_tile_tex.x + tile_size.x, curent_tile_tex.y);
			current_tile_quad[2].texCoords = sf::Vector2f(curent_tile_tex.x + tile_size.x, curent_tile_tex.y + tile_size.y);
			current_tile_quad[3].texCoords = sf::Vector2f(curent_tile_tex.x, curent_tile_tex.y + tile_size.y);

			if (show_cooliders && world_map[current_tile_idx].is_collidable)
			{
				current_tile_quad[0].color = sf::Color::Green;
				current_tile_quad[1].color = sf::Color::Green;
				current_tile_quad[2].color = sf::Color::Green;
				current_tile_quad[3].color = sf::Color::Green;
			}
		}
	}
}

void aco::tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}
