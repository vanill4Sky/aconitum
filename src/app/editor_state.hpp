#pragma once

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

void integer_round(int& num, int step);

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

	app_data& m_app_data;
	grid m_grid;
	std::unique_ptr<level> m_level;
	std::unique_ptr<tile_picker> m_tile_picker;
	sf::RenderStates m_world_render_states;

	int m_current_zoom;
	std::unordered_map<sf::Mouse::Button, bool> m_mouse_state;
	sf::Vector2f m_origin_tile_coords;
	sf::Vector2f m_hovered_tile_coords;
	sf::Vector2f m_prev_tile_delta;
	sf::Vector2i m_render_tile_translation;
	bool m_is_grid_visible{ true };
	int m_brush_mode;
	int m_horizontal_bounds_input[2];
	int m_vertical_bounds_input[2];

	float zoom_factor{ 1.1f };
	float speed{ 32.0f };

	sf::Texture tileset;
	sf::RectangleShape debug_follower;
};

}