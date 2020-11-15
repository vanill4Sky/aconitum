#include "level.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "constants.hpp"

namespace fs = std::filesystem;

aco::level::level(float tile_size)
	: m_tile_size{ tile_size }
	, m_width{ 0 }
	, m_height{ 0 }
{
}

aco::tile& aco::level::at(aco::layer layer, int pos_x, int pos_y)
{
	return at(m_data.get(layer), pos_x, pos_y);
}

const aco::tile& aco::level::at(aco::layer layer, int pos_x, int pos_y) const
{
	return at(m_data.get(layer), pos_x, pos_y);
}

void aco::level::update_tilemap()
{
	m_tilemap.get(aco::layer::bottom) = aco::tilemap(
		m_tileset, { m_tile_size, m_tile_size }, m_data.get(aco::layer::bottom), m_width, m_height);
	m_tilemap.get(aco::layer::top) = aco::tilemap(
		m_tileset, { m_tile_size, m_tile_size }, m_data.get(aco::layer::top), m_width, m_height);
	m_collider_mask = aco::tilemap(
		sf::Color{0, 100, 0, 125}, { m_tile_size, m_tile_size }, m_data.get(aco::layer::bottom), m_width, m_height);
}

void aco::level::draw(sf::RenderWindow& render_window, bool show_collider_mask) const
{
	render_window.draw(m_tilemap.get(aco::layer::bottom), m_level_render_states);
	render_window.draw(m_tilemap.get(aco::layer::top), m_level_render_states);
	if (show_collider_mask)
	{
		render_window.draw(m_collider_mask, m_level_render_states);
	}
}

void aco::level::set_filename(std::string filename)
{
	m_filename = std::move(filename);
}

void aco::level::write_to_file(std::filesystem::path levels_dir) const
{
	if(!m_filename.empty())
	{
		std::ofstream output(levels_dir /= std::filesystem::path(m_filename));
		nlohmann::json json;
		json["width"] = m_width;
		json["height"] = m_height;
		json["tile_size"] = m_tile_size;
		json["bottom_layer_data"] = m_data.bottom;
		json["top_layer_data"] = m_data.top;
		json["tileset_filename"] = m_tileset_filename;
		output << json << '\n';
	}
}

void aco::level::read_from_file(const std::filesystem::path& path)
{
	m_filename = path.filename().string();

	std::ifstream input(path);
	nlohmann::json json;
	input >> json;
	m_width = json["width"].get<size_t>();
	m_height = json["height"].get<size_t>();
	m_tile_size = json["tile_size"].get<float>();
	m_data.bottom = json["bottom_layer_data"].get<std::vector<aco::tile>>();
	m_data.top = json["top_layer_data"].get<std::vector<aco::tile>>();
	
	auto tileset_filename{ json["tileset_filename"].get<std::string>() };
	load_tileset(tileset_dir + tileset_filename);

	update_tilemap();
}

bool aco::level::load_tileset(const std::string& tileset_path)
{
	if (m_tileset.loadFromFile(tileset_path))
	{
		m_tileset_filename = fs::path{ tileset_path }.filename().string();
		update_tilemap();
		return true;
	}
	return false;
}

const sf::RenderStates& aco::level::level_render_states() const
{
	return m_level_render_states;
}

sf::Vector2i aco::level::render_translation_tiles() const
{
	auto matrix{ m_level_render_states.transform.getMatrix() };
	return static_cast<sf::Vector2i>((sf::Vector2f{ matrix[12], matrix[13] } / m_tile_size));
}

sf::Vector2f aco::level::render_translation() const
{
	auto matrix{ m_level_render_states.transform.getMatrix() };
	return sf::Vector2f{ matrix[12], matrix[13] };
}

float aco::level::tile_size() const
{
	return m_tile_size;
}

const std::string& aco::level::filename() const
{
	return m_filename;
}

size_t aco::level::width() const
{
	return m_width;
}

size_t aco::level::height() const
{
	return m_height;
}

const sf::Texture& aco::level::tileset() const
{
	return m_tileset;
}

const std::string& aco::level::tileset_filename() const
{
	return m_tileset_filename;
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

const aco::tile& aco::level::at(const std::vector<aco::tile>& layer_data, int pos_x, int pos_y) const
{
	assert(pos_x >= 0 && pos_x < m_width);
	assert(pos_y >= 0 && pos_y < m_height);

	return layer_data[pos_y * m_width + pos_x];
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

void aco::level::optimize_size()
{
	const auto find_layer_bbox = [&](const auto& layer) {
		sf::Rect<size_t> layer_bbox(
			std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(),
			std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::min());
		for (size_t y{ 0 }; y < m_height; ++y)
		{
			for (size_t x{ 0 }; x < m_width; ++x)
			{
				if (!layer[y * m_width + x].is_blank)
				{
					layer_bbox.left = std::min(layer_bbox.left, x);
					layer_bbox.top = std::min(layer_bbox.top, y);

					layer_bbox.width = std::max(layer_bbox.width, x - layer_bbox.left + 1);
					layer_bbox.height = std::max(layer_bbox.height, y - layer_bbox.top + 1);
				}
			}
		}

		return layer_bbox;
	};

	const auto is_empty = [](const auto& bbox) {
		return bbox.width == 0 && bbox.height == 0;
	};

	sf::Rect<size_t> level_bbox;
	auto bottom_layer_bbox{ find_layer_bbox(m_data.bottom) };
	auto top_layer_bbox{ find_layer_bbox(m_data.top) };
	if (!is_empty(bottom_layer_bbox) && !is_empty(top_layer_bbox))
	{
		level_bbox.left = std::min(bottom_layer_bbox.left, top_layer_bbox.left);
		level_bbox.top = std::min(bottom_layer_bbox.top, top_layer_bbox.top);
		level_bbox.width = std::max(bottom_layer_bbox.left + bottom_layer_bbox.width,
			top_layer_bbox.left + top_layer_bbox.width) - level_bbox.left;
		level_bbox.height = std::max(bottom_layer_bbox.top + bottom_layer_bbox.height,
			top_layer_bbox.top + top_layer_bbox.height) - level_bbox.top;
	}
	else if (!is_empty(bottom_layer_bbox) && is_empty(top_layer_bbox))
	{
		level_bbox = bottom_layer_bbox;
	}
	else if (is_empty(bottom_layer_bbox) && !is_empty(top_layer_bbox))
	{
		level_bbox = top_layer_bbox;
	}

	const auto crop_layer = [&](auto& layer) {
		for (size_t y{ 0 }; y < level_bbox.height; ++y)
		{
			for (size_t x{ 0 }; x < level_bbox.width; ++x)
			{
				const auto new_idx{ y * level_bbox.width + x };
				const auto old_idx{ ((y + level_bbox.top) * m_width) + (x + level_bbox.left) };
				layer[new_idx] = layer[old_idx];
			}
		}
		if (const auto offset{ layer.begin() + level_bbox.height * level_bbox.width };
			offset != layer.end())
		{
			layer.erase(offset);
		}
	};

	crop_layer(m_data.bottom);
	crop_layer(m_data.top);
	m_width = level_bbox.width;
	m_height = level_bbox.height;

	update_tilemap();
}

void aco::level::reset_to_origin()
{
	m_level_render_states = sf::RenderStates();
}

void aco::to_json(nlohmann::json& j, const aco::tile& tile)
{
	j = nlohmann::json{ tile.is_blank, tile.is_collidable, tile.tex_coords.x, tile.tex_coords.y };
}

void aco::from_json(const nlohmann::json& j, aco::tile& tile)
{
	j.at(0).get_to(tile.is_blank);
	j.at(1).get_to(tile.is_collidable);
	j.at(2).get_to(tile.tex_coords.x);
	j.at(3).get_to(tile.tex_coords.y);
}
