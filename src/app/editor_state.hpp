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
#include "constants.hpp"

namespace aco
{
using text_input_buffer = std::array<char, 256>;

struct entity_stub
{
	static std::string generate_id(const std::string& name);

	entity_stub(std::string id, std::string name, const sf::Sprite& sprite);
	std::string id;
	std::string name;
	sf::RectangleShape shape;

private:
	static std::unordered_map<std::string, int> entity_count;
};

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
	void update_level_editor_tab();
	void update_entity_placer_tab();
	void update_tile(sf::Vector2i mouse_position, float tile_size);
	void init_level();
	void load_level();
	void save_level();

	void get_available_templates(const std::string& index_filename);
	void load_template();
	std::string current_entity_name() const;
	void highlight_entity(int index, bool is_highlighted);

	app_data& m_app_data;
	grid m_grid;
	std::unique_ptr<level> m_level;
	tile_picker m_tile_picker;

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
	std::vector<std::string> m_tileset_files_list;
	int m_current_level_file_idx;
	int m_current_tileset_file_idx;
	text_input_buffer m_new_level_name;
	text_input_buffer m_save_level_name;
	bool m_is_level_editor_visible{ true };

	sol::table m_entities;
	std::vector<std::string> m_entity_names_list;
	int m_current_entity_file_idx;
	sf::Sprite m_miniature;
	std::vector<entity_stub> m_stubs;
	sf::Vector2f m_prev_mouse_pos;
	int m_selected_entity_idx{ -1 };

	float zoom_factor{ 1.1f };

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
	static auto vector_of_stubs_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<aco::entity_stub>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).id.c_str();
		return true;
	};
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);
	bool ListBox(const char* label, int* currIndex, std::vector<aco::entity_stub>& values);
}