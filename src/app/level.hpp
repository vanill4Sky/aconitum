#pragma once

#include <filesystem>
#include <vector>
#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../core/tilemap.hpp"

namespace aco
{

void to_json(nlohmann::json& j, const aco::tile& tile);
void from_json(const nlohmann::json& j, aco::tile& tile);

enum class layer
{
	bottom,
	top
};

template<typename T>
struct pair
{
	T bottom;
	T top;
	T& get(aco::layer layer);
	const T& get(aco::layer layer) const;
};

class level
{
public:
	level(sf::Texture tileset, float tile_size);
	level(sf::Texture tileset, float tile_size, size_t width, size_t height);

	aco::tile& at(aco::layer layer, int pos_x, int pos_y);
	const sf::RenderStates& level_render_states() const;
	const sf::Vector2i render_translation() const;
	float tile_size() const;
	const std::string& filename() const;

	void move(sf::Vector2f delta);
	void update_tilemap(bool show_colliders = false);
	void draw(sf::RenderWindow& render_window) const;
	void set_filename(std::string filename);

	void write_to_file(std::filesystem::path levels_dir) const;
	void read_from_file(const std::filesystem::path& path);

private:
	aco::tile& at(std::vector<aco::tile>& layer_data, int pos_x, int pos_y);
	void resize(size_t new_width, size_t new_height,
		size_t offset_x = 0, size_t offset_y = 0);
	void resize(std::vector<aco::tile>& layer_data, size_t new_width, size_t new_height,
		size_t offset_x = 0, size_t offset_y = 0);

	std::string m_filename;
	sf::Texture m_tileset;
	pair<aco::tilemap> m_tilemap;
	pair<std::vector<aco::tile>> m_data;
	sf::RenderStates m_level_render_states;
	float m_tile_size;
	size_t m_width;
	size_t m_height;
};

}

template<typename T>
inline T& aco::pair<T>::get(aco::layer layer)
{
	switch (layer)
	{
	case aco::layer::bottom:
		return bottom;
	case aco::layer::top:
		return top;
	default:
		return bottom;
	}
}

template<typename T>
inline const T& aco::pair<T>::get(aco::layer layer) const
{
	switch (layer)
	{
	case aco::layer::bottom:
		return bottom;
	case aco::layer::top:
		return top;
	default:
		return bottom;
	}
}
