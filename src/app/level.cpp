#include "level.hpp"

#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

aco::level::level(sf::Texture tileset, float tile_size, size_t width, size_t height)
	: m_tileset{ std::move(tileset) }
	, m_tile_size{ tile_size }
	, m_width{ width }
	, m_height{ height }
{
	read_from_file("assets/levels/test_01.json");
}

aco::tile& aco::level::at(aco::layer layer, int pos_x, int pos_y)
{
	return at(m_data.get(layer), pos_x, pos_y);
}

void aco::level::update_tilemap()
{
	m_tilemap.get(aco::layer::bottom) = aco::tilemap(
		m_tileset, { m_tile_size, m_tile_size }, m_data.get(aco::layer::bottom), m_width, m_height);
	m_tilemap.get(aco::layer::top) = aco::tilemap(
		m_tileset, { m_tile_size, m_tile_size }, m_data.get(aco::layer::top), m_width, m_height);

	write_to_file("assets/levels/test_01.json");
}

void aco::level::draw(sf::RenderWindow& render_window) const
{
	render_window.draw(m_tilemap.get(aco::layer::bottom), m_level_render_states);
	render_window.draw(m_tilemap.get(aco::layer::top), m_level_render_states);
}

void aco::level::write_to_file(const std::filesystem::path& path) const
{
	std::ofstream output(path);
	nlohmann::json json;
	json["width"] = m_width;
	json["height"] = m_height;
	json["tile_size"] = m_tile_size;
	json["bottom_layer_data"] = m_data.bottom;
	json["top_layer_data"] = m_data.top;
	output << json << '\n';
}

void aco::level::read_from_file(const std::filesystem::path& path)
{
	std::ifstream input(path);
	nlohmann::json json;
	input >> json;
	m_width = json["width"].get<size_t>();
	m_height = json["height"].get<size_t>();
	m_tile_size = json["tile_size"].get<float>();
	m_data.bottom = json["bottom_layer_data"].get<std::vector<aco::tile>>();
	m_data.top = json["top_layer_data"].get<std::vector<aco::tile>>();
	update_tilemap();
}

const sf::RenderStates& aco::level::level_render_states() const
{
	return m_level_render_states;
}

const sf::Vector2i aco::level::render_translation() const
{
	auto matrix{ m_level_render_states.transform.getMatrix() };
	return static_cast<sf::Vector2i>((sf::Vector2f{ matrix[12], matrix[13] } / m_tile_size));
}

float aco::level::tile_size() const
{
	return m_tile_size;
}

aco::tile& aco::level::at(std::vector<aco::tile>& layer_data, int pos_x, int pos_y)
{
	size_t abs_pos_x{ static_cast<size_t>(std::abs(pos_x)) };
	size_t abs_pos_y{ static_cast<size_t>(std::abs(pos_y)) };

	if (pos_x < 0 && pos_y < 0)
	{
		resize(m_width + abs_pos_x, m_height + abs_pos_y, abs_pos_x, abs_pos_y);
		abs_pos_x = 0;
		abs_pos_y = 0;
		move({ static_cast<float>(pos_x), static_cast<float>(pos_y) });
	}
	else if (pos_x < 0)
	{
		resize(m_width + abs_pos_x, m_height, abs_pos_x, 0);
		abs_pos_x = 0;
		move({ static_cast<float>(pos_x), 0.0f });
	}
	else if (pos_y < 0)
	{
		resize(m_width, m_height + abs_pos_y, 0, abs_pos_y);
		abs_pos_y = 0;
		move({ 0.0f, static_cast<float>(pos_y) });
	}

	if (abs_pos_x >= m_width || abs_pos_y >= m_height)
	{
		resize(std::max(abs_pos_x + 1, m_width), std::max(abs_pos_y + 1, m_height));
	}

	return layer_data[abs_pos_y * m_width + abs_pos_x];
}

void aco::level::move(sf::Vector2f delta)
{
	m_level_render_states.transform.translate(m_tile_size * delta);
}

void aco::level::resize(size_t new_width, size_t new_height,
	size_t offset_x, size_t offset_y)
{
	resize(m_data.get(aco::layer::bottom), new_width, new_height, offset_x, offset_y);
	resize(m_data.get(aco::layer::top), new_width, new_height, offset_x, offset_y);
	m_width = new_width;
	m_height = new_height;

	spdlog::info("current world size in tiles ({}, {})", m_width, m_height);
}

void aco::level::resize(std::vector<aco::tile>& layer_data, size_t new_width, size_t new_height, 
	size_t offset_x, size_t offset_y)
{
	std::vector<aco::tile> temp{ new_width * new_height };
	auto horizontal_limit{ std::min(m_width, new_width) };
	auto vertical_limit{ std::min(m_height, new_height) };

	for (size_t y{ 0 }; y < vertical_limit; ++y)
	{
		for (size_t x{ 0 }; x < horizontal_limit; ++x)
		{
			temp[(y + offset_y) * new_width + x + offset_x] = layer_data[y * m_width + x];
		}
	}

	layer_data = std::move(temp);
}

void aco::to_json(nlohmann::json& j, const aco::tile& tile)
{
	//j = nlohmann::json{ 
	//	{"is_blank", tile.is_blank}, 
	//	{"tex_x", tile.tex_coords.x}, {"tex_y", tile.tex_coords.y} 
	//};
	j = nlohmann::json{ tile.is_blank, tile.tex_coords.x, tile.tex_coords.y };
}

void aco::from_json(const nlohmann::json& j, aco::tile& tile)
{
	j.at(0).get_to(tile.is_blank);
	j.at(1).get_to(tile.tex_coords.x);
	j.at(2).get_to(tile.tex_coords.y);
}
