#include "editor_state.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Window/Event.hpp>

#include <spdlog/spdlog.h>

aco::editor_state::editor_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, tile_size{ 32 }
	, m_grid{ sf::Vector2f(32, 32), static_cast<sf::Vector2f>(m_app_data.window.getSize()), sf::Color::Red }
	, debug_pointer{ sf::Vector2f(tile_size, tile_size) }
	, debug_follower{ sf::Vector2f(tile_size, tile_size) }
{
	ImGui::SFML::Init(app_data.window);

	tileset.loadFromFile("assets/textures/tileset_dungeon_01.png");
	tiles_vertically = tileset.getSize().y / tile_size;
	tiles_horizontally = tileset.getSize().x / tile_size;

	tile.setTexture(tileset);
	for (int i = 0; i < tiles_horizontally; ++i)
	{
		for (int j = 0; j < tiles_vertically; ++j)
		{
			tile_picker.emplace_back(tileset, sf::IntRect{ i * tile_size , j * tile_size , tile_size , tile_size });
		}
	}

	debug_pointer.setFillColor(sf::Color::Red);
	debug_follower.setFillColor(sf::Color(0, 255, 0, 63));
}

aco::editor_state::~editor_state()
{
	ImGui::SFML::Shutdown();
}

void aco::editor_state::init()
{

}

void aco::editor_state::handle_input()
{
	const auto integer_round = [](int& num, int step) {
		int sgn = (num >= 0) ? 1 : -1;
		num = (num + sgn * step / 2) / step;
	};

	sf::Event event;
	while (m_app_data.window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) 
		{
			m_app_data.window.close();
		}
		else if (event.type == sf::Event::Resized)
		{
			sf::View view{ sf::FloatRect(0.f, 0.f, event.size.width, event.size.height) };
			view.zoom(1.0f / current_zoom);
			m_app_data.window.setView(view);

			m_grid.resize(static_cast<sf::Vector2f>(m_app_data.window.getSize()));
		}
		else if (event.type == sf::Event::MouseWheelScrolled && !ImGui::IsAnyWindowHovered())
		{
			auto view{ m_app_data.window.getView() };
			const auto zoom_factor{ 1.1 };

			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
			{
				if (event.mouseWheelScroll.delta > 0)
				{
					view.zoom(1.0f / zoom_factor);
					current_zoom *= zoom_factor;
				}
				else
				{
					view.zoom(zoom_factor);
					current_zoom *= (1.0f / zoom_factor);
				}
			}

			spdlog::debug("current zoom: {}, relative tile size: {}", current_zoom, current_zoom * tile_size);

			m_app_data.window.setView(view);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				is_left_mouse_button_pressed = true;
				mouse_click_origin = sf::Vector2i{ event.mouseButton.x, event.mouseButton.y };
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				sf::Vector2i mouse_pixel_pos{ sf::Mouse::getPosition(m_app_data.window) };
				spdlog::debug("action key click pixel (x, y): ({}, {})", mouse_pixel_pos.x, mouse_pixel_pos.y);
				sf::Vector2f mouse_world_pos{ m_app_data.window.mapPixelToCoords(mouse_pixel_pos) };
				spdlog::debug("action key click world (x, y): ({}, {})", mouse_world_pos.x, mouse_world_pos.y);
				sf::Vector2f tile_coords{ std::floor(mouse_world_pos.x / tile_size), std::floor(mouse_world_pos.y / tile_size) };
				spdlog::debug("tile_cords (x, y): ({}, {})", tile_coords.x, tile_coords.y);

				debug_pointer.setPosition(tile_coords * static_cast<float>(tile_size));
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				is_left_mouse_button_pressed = false;
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (is_left_mouse_button_pressed && !ImGui::IsAnyItemActive())
			{
				const sf::Vector2i current_mouse_position{ event.mouseMove.x, event.mouseMove.y };
				sf::Vector2i mouse_position_delta{ current_mouse_position - mouse_click_origin };
				const auto relative_tile_size{ static_cast<int>(tile_size * current_zoom) };
				integer_round(mouse_position_delta.x, relative_tile_size);
				integer_round(mouse_position_delta.y, relative_tile_size);

				const auto speed{ static_cast<float>(tile_size) };
				if (mouse_position_delta.x != 0)
				{
					word_render_states.transform.translate({ speed * mouse_position_delta.x, 0.0f });
					mouse_click_origin.x += relative_tile_size * mouse_position_delta.x;

					auto matrix{ word_render_states.transform.getMatrix() };
					spdlog::debug("tx: {} ty: {}", matrix[12], matrix[13]);
				}

				if (mouse_position_delta.y != 0)
				{
					word_render_states.transform.translate({ 0.0f, speed * mouse_position_delta.y });
					mouse_click_origin.y += relative_tile_size * mouse_position_delta.y;

					auto matrix{ word_render_states.transform.getMatrix() };
					spdlog::debug("tx: {} ty: {}", matrix[12], matrix[13]);
				}
			}

			sf::Vector2i mouse_pixel_pos{ sf::Mouse::getPosition(m_app_data.window) };
			sf::Vector2f mouse_world_pos{ m_app_data.window.mapPixelToCoords(mouse_pixel_pos) };
			sf::Vector2f tile_coords{ std::floor(mouse_world_pos.x / tile_size), std::floor(mouse_world_pos.y / tile_size) };

			debug_follower.setPosition(tile_coords * static_cast<float>(tile_size));
		}
	}

}

void aco::editor_state::update(float dt)
{
	ImGui::SFML::Update(m_app_data.window, sf::seconds(dt));

	ImGui::ShowTestWindow();

	ImGui::SetNextWindowPos({ m_app_data.window.getSize().x - 300.0f, 0.0f });
	ImGui::SetNextWindowSize({ 300.0f, m_app_data.window.getSize().y + 0.0f });
	ImGui::Begin("Level editor", nullptr, ImGuiWindowFlags_NoResize);
	for (int i = 0; i < tiles_horizontally; ++i)
	{
		for (int j = 0; j < tiles_vertically; ++j)
		{
			int idx{ i + j * tiles_horizontally };
			ImGui::PushID(idx);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
			ImGui::ImageButton(tile_picker[idx], 1);
			ImGui::PopStyleVar();

			ImGui::PopID();
			ImGui::SameLine(0.0f, 1.0f);
		}
		ImGui::NewLine();
	}
	ImGui::End();

	ImGui::EndFrame();
}

void aco::editor_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(tile, word_render_states);
	m_app_data.window.draw(m_grid);
	m_app_data.window.draw(debug_pointer);
	m_app_data.window.draw(debug_follower);
	ImGui::SFML::Render(m_app_data.window);
	m_app_data.window.display();
}
