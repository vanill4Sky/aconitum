#pragma once

#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../core/state.hpp"
#include "../core/grid.hpp"
#include "../core/tilemap.hpp"
#include "app_data.hpp"
#include "level.hpp"

namespace aco
{

void integer_round(int& num, int step);

class tile_picker
{
public:
	tile_picker(const sf::Texture& tileset, sf::Vector2f tile_size);
	tile_picker(const sf::Texture& tileset, float tile_size);

	sf::Sprite at(size_t pos_x, size_t pos_y) const;
	sf::Vector2<size_t> tile_count() const;
	size_t width() const;
	size_t height() const;

	void set_active_tile(size_t pos_x, size_t pos_y);
	void update_active_tile(int delta_x, int delta_y);
	sf::Vector2<size_t> active_tile() const;

private:
	sf::Vector2<size_t> calc_tile_count(const sf::Texture& tileset, sf::Vector2f tile_size);
	std::vector<sf::Sprite> split_tileset(const sf::Texture& tileset, sf::Vector2i tile_size);

	sf::Vector2<size_t> m_tile_count;
	std::vector<sf::Sprite> m_data;
	sf::Vector2<size_t> m_active_tile;
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

	sf::View resize_current_view(sf::Vector2u new_size);
	void zoom(int delta);

	app_data& m_app_data;
	grid m_grid;
	std::unique_ptr<level> m_level;
	std::unique_ptr<tile_picker> m_tile_picker;

	int m_current_zoom;
	std::unordered_map<sf::Mouse::Button, bool> m_mouse_state;
	sf::Vector2f m_origin_tile_coords;
	sf::Vector2f m_render_translation;
	sf::Vector2f m_mouse_world_position;
	sf::Vector2f m_hovered_tile_coords;
	sf::Vector2f m_prev_tile_delta;
	bool m_is_grid_visible{ true };

	int tile_size;
	sf::RenderStates word_render_states;
	sf::Texture tileset;
	bool is_left_mouse_button_pressed{ false };
	sf::Sprite tile;
	std::vector<sf::Sprite> tile_picker;
	sf::RectangleShape debug_follower;
	aco::tilemap m_tilemap;

	float zoom_factor{ 1.1f };
	float speed{ 32.0f };
};

}