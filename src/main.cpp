#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <entt.hpp>
#include <sol/sol.hpp>

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

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// catch the resize events
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));

				//grid = generate_grid({ tile_size, tile_size }, window.getSize());
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
		window.draw(tile);
		window.draw(grid);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}