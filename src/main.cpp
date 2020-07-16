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
	entt::registry registry;

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file("assets/scripts/test.lua");

	sf::RenderWindow window(sf::VideoMode(1500, 1000), "ImGui + SFML = <3");
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
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));

				grid.resize(static_cast<sf::Vector2f>(window.getSize()));
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				auto view{ window.getView() };
				const auto speed{ static_cast<float>(tile_size) };
				const auto zoom_factor{ 1.2f };

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
				case sf::Keyboard::A:
					//word_render_states.transform.scale({ zoom_factor , zoom_factor });
					//grid_render_states.transform.scale({ zoom_factor , zoom_factor });
					view.zoom(1.0f / zoom_factor);
					break;
				case sf::Keyboard::S:
					//word_render_states.transform.scale({ 1.0f / zoom_factor , 1.0f / zoom_factor });
					//grid_render_states.transform.scale({ 1.0f / zoom_factor , 1.0f / zoom_factor });
					view.zoom(zoom_factor);
					break;
				}

				window.setView(view);
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::ShowTestWindow();

		std::string res = lua["hello_world"]();
		ImGui::Begin("Level editor");

		for (int i = 0; i < tiles_horizontally; ++i)
		{
			for (int j = 0; j < tiles_vertically; ++j)
			{
				int idx{ i + j * tiles_horizontally };
				ImGui::PushID(idx);
				ImGui::ImageButton(tile_picker[idx], 1);

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