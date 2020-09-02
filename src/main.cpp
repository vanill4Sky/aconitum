#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <entt.hpp>
#include <sol/sol.hpp>
#include <spdlog/spdlog.h>

#include "core/grid.hpp"

int main()
{
	spdlog::set_level(spdlog::level::debug);

	entt::registry registry;

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file("assets/scripts/test.lua");

	sf::RenderWindow window(sf::VideoMode(1500, 1000), "aconitum_wip");
	window.setPosition(sf::Vector2i(1920 + (1920 - window.getSize().x) / 2, 0));
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	constexpr int tile_size{ 32 };
	sf::Texture tileset;
	tileset.loadFromFile("assets/textures/tileset_dungeon_01.png");
	int tiles_vertically = tileset.getSize().y / tile_size;
	int tiles_horizontally = tileset.getSize().x / tile_size;

	sf::Sprite tile{ tileset };

	std::vector<sf::Sprite> tile_picker;

	for (int i = 0; i < tiles_horizontally; ++i)
	{
		for (int j = 0; j < tiles_vertically; ++j)
		{
			tile_picker.emplace_back(tileset, sf::IntRect{ i * tile_size , j * tile_size , tile_size , tile_size });
		}
	}

	aco::grid grid{ sf::Vector2f(tile_size, tile_size), static_cast<sf::Vector2f>(window.getSize()), sf::Color::Red };

	sf::RenderStates word_render_states;
	sf::RenderStates grid_render_states;

	bool is_left_mouse_button_pressed{ false };
	sf::Vector2i mouse_click_origin;

	auto current_zoom{ 1.0f };

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visible_area(0.f, 0.f, event.size.width, event.size.height);
				sf::View view{ visible_area };
				view.zoom(1.0f / current_zoom);
				window.setView(view);

				grid.resize(static_cast<sf::Vector2f>(window.getSize()));
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
				auto view{ window.getView() };
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

				window.setView(view);
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
						word_render_states.transform.translate({ speed * mouse_position_delta.x, 0.0f});
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

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::ShowTestWindow();

		std::string res = lua["hello_world"]();

		ImGui::SetNextWindowPos({ window.getSize().x - 300.0f, 0.0f });
		ImGui::SetNextWindowSize({ 300.0f, window.getSize().y + 0.0f });
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

		window.clear();
		window.draw(tile, word_render_states);
		window.draw(grid, grid_render_states);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}