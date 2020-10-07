#pragma once

#include <array>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../core/state.hpp"
#include "../core/grid.hpp"
#include "../core/tilemap.hpp"
#include "app_data.hpp"
#include "level.hpp"
#include "tile_picker.hpp"

namespace aco
{

const std::string levels_dir{ "assets/levels/" };

using text_input_buffer = std::array<char, 256>;

class editor_state : public state
{
public:
	editor_state(aco::app_data& app_data);
	~editor_state();

	void init();
	void handle_input();
	void update(float dt);
	void draw();

private:
	void handle_resize_event(const sf::Event::SizeEvent& event);
	void handle_zoom_event(const sf::Event::MouseWheelScrollEvent& event);
	void handle_mouse_click(const sf::Event::MouseButtonEvent& event);
	void handle_mouse_move_event(const sf::Event::MouseMoveEvent& event);
	void validate_bounds_input();
	sf::View resize_current_view(sf::Vector2u new_size);
	void zoom(int delta);
	sf::Vector2f calc_tile_coordinates(sf::Vector2i mouse_position, float tile_size) const;
	sf::Vector2i calc_tile_world_coordinates(sf::Vector2i mouse_position, float tile_size);
	void update_tile(sf::Vector2i mouse_position, float tile_size);
	void init_level();
	void load_level();
	void save_level();

	app_data& m_app_data;
	grid m_grid;
	std::unique_ptr<level> m_level;
	std::unique_ptr<tile_picker> m_tile_picker;

	int m_current_zoom;
	std::unordered_map<sf::Mouse::Button, bool> m_mouse_state;
	sf::Vector2f m_origin_tile_coords;
	sf::Vector2f m_hovered_tile_coords;
	sf::Vector2f m_prev_tile_delta;
	bool m_is_grid_visible{ true };
	bool m_is_collider_visible{ false };
	bool m_is_adding_collider{ false };
	int m_brush_mode;
	int m_horizontal_bounds_input[2];
	int m_vertical_bounds_input[2];
	int m_current_layer;
	std::vector<std::string> m_level_files_list;
	int m_current_level_file_idx;
	text_input_buffer m_new_level_name;
	text_input_buffer m_save_level_name;

	float zoom_factor{ 1.1f };

	sf::Texture tileset;
	sf::RectangleShape debug_follower;
};

}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);
}