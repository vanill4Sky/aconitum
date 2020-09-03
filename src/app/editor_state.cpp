#include "editor_state.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Window/Event.hpp>

#include <spdlog/spdlog.h>

aco::editor_state::editor_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, tile_size{ 32 }
	, m_grid{ sf::Vector2f(32, 32), static_cast<sf::Vector2f>(m_app_data.window.getSize()), sf::Color::Red }
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
		else if (event.type == sf::Event::KeyPressed)
		{
			const auto speed{ static_cast<float>(tile_size) };

			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				word_render_states.transform.translate({ 0.0f, -speed });
				break;
			case sf::Keyboard::Down:
				word_render_states.transform.translate({ 0.0f, speed });
				break;
			case sf::Keyboard::Right:
				word_render_states.transform.translate({ speed, 0.0f });
				break;
			case sf::Keyboard::Left:
				word_render_states.transform.translate({ -speed, 0.0f });
				break;
			}
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
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
				const auto integer_round = [](int& num, int step) {
					int sgn = (num >= 0) ? 1 : -1;
					num = (num + sgn * step / 2) / step;
				};

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
				}

				if (mouse_position_delta.y != 0)
				{
					word_render_states.transform.translate({ 0.0f, speed * mouse_position_delta.y });
					mouse_click_origin.y += relative_tile_size * mouse_position_delta.y;
				}
			}
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
	ImGui::SFML::Render(m_app_data.window);
	m_app_data.window.display();
}
