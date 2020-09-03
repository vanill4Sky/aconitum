#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../core/state.hpp"
#include "../core/grid.hpp"
#include "app_data.hpp"

namespace aco
{

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
	app_data& m_app_data;
	grid m_grid;
	float current_zoom{ 1.0f };
	int tile_size;
	sf::RenderStates word_render_states;
	sf::Texture tileset;
	bool is_left_mouse_button_pressed{ false };
	sf::Vector2i mouse_click_origin;
	sf::Sprite tile;
	std::vector<sf::Sprite> tile_picker;
	int tiles_vertically;
	int tiles_horizontally;
};

}