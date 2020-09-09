#include "editor_state.hpp"

#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>

aco::editor_state::editor_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, m_grid{ sf::Vector2f(32, 32), static_cast<sf::Vector2f>(m_app_data.window.getSize()), sf::Color::Red }
	, m_current_zoom{ 0 }
	, m_mouse_click_origin{ 0, 0 }
	, m_render_translation{ 0.0f, 0.0f }
	, tile_size{ 32 }
	, debug_follower{ sf::Vector2f(tile_size, tile_size) }
{
	ImGui::SFML::Init(app_data.window);

	tileset.loadFromFile("assets/textures/tileset_dungeon_01.png");
	m_level = std::make_unique<aco::level>(tileset, 32.0f, 10, 10);
	m_tilemap = m_level->get_tilemap();
	m_tile_picker = std::make_unique<aco::tile_picker>(tileset, 32.0f);

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
			handle_resize_event(event.size);
		}
		else if (event.type == sf::Event::MouseWheelScrolled && !ImGui::IsAnyWindowHovered())
		{
			handle_zoom_event(event.mouseWheelScroll);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			m_mouse_state[event.mouseButton.button] = true;

			handle_mouse_click(event.mouseButton);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			m_mouse_state[event.mouseButton.button] = false;
		}
		else if (event.type == sf::Event::MouseMoved && !ImGui::IsAnyItemActive())
		{
			handle_mouse_move_event(event.mouseMove);
		}
	}

}

void aco::editor_state::update(float dt)
{
	ImGui::SFML::Update(m_app_data.window, sf::seconds(dt));

	ImGui::ShowTestWindow();

	ImGui::SetNextWindowPos({ m_app_data.window.getSize().x - 300.0f, 0.0f });
	ImGui::SetNextWindowSize({ 300.0f, /*m_app_data.window.getSize().y +*/ 0.0f });
	ImGui::Begin("Level editor", nullptr, ImGuiWindowFlags_NoResize);
	for (size_t y = 0; y < m_tile_picker->height(); ++y)
	{
		for (size_t x = 0; x < m_tile_picker->width(); ++x)
		{
			int id{ static_cast<int>(y * m_tile_picker->width() + x) };
			ImGui::PushID(id);
			if (sf::Vector2<size_t> current_tile{ x, y }; 
				m_tile_picker->active_tile() == current_tile)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
				ImGui::PushStyleColor(ImGuiCol_Border, { 1.0f, 0.0f, 0.0f, 1.0f });
			}
			else
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleColor(ImGuiCol_Border, { 0.0f, 0.0f, 0.0f, 1.0f });
			}
			if (ImGui::ImageButton(m_tile_picker->at(x, y), 1))
			{
				m_tile_picker->set_active_tile(x, y);
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			ImGui::PopID();
			ImGui::SameLine(0.0f, 1.0f);
		}
		ImGui::NewLine();
	}
	ImGui::Separator();

	ImGui::Checkbox("Show grid", &m_is_grid_visible);
	ImGui::Separator();

	ImGui::Text("Mouse position (world): (%.1f, %.1f)", 
		m_mouse_world_position.x, m_mouse_world_position.y);
	ImGui::Text("Hovered tile coordinates: (%.0f, %.0f)", 
		m_hovered_tile_coords.x, m_hovered_tile_coords.y);
	ImGui::Text("Render translation (pixels): (%.0f, %.0f)",
		m_render_translation.x, m_render_translation.y);

	ImGui::End();

	ImGui::EndFrame();

	m_grid.set_visible(m_is_grid_visible);
}

void aco::editor_state::draw()
{
	m_app_data.window.clear();
	m_app_data.window.draw(m_tilemap, word_render_states);
	m_app_data.window.draw(m_grid);
	m_app_data.window.draw(debug_follower);
	ImGui::SFML::Render(m_app_data.window);
	m_app_data.window.display();
}

void aco::editor_state::handle_resize_event(const sf::Event::SizeEvent& event)
{
	m_app_data.window.setView(resize_current_view({ event.width, event.height }));
	zoom(0);

	m_grid.resize(static_cast<sf::Vector2f>(m_app_data.window.getSize()));
}

void aco::editor_state::handle_zoom_event(const sf::Event::MouseWheelScrollEvent& event)
{
	if (event.wheel == sf::Mouse::VerticalWheel)
	{
		zoom(static_cast<int>(event.delta));
	}
}

void aco::editor_state::handle_mouse_click(const sf::Event::MouseButtonEvent& event)
{
	if (event.button == sf::Mouse::Left)
	{
		m_mouse_click_origin = sf::Vector2i{ event.x, event.y };
	}
	else if (event.button == sf::Mouse::Right)
	{
		sf::Vector2i selected_tile(
			static_cast<sf::Vector2i>(m_hovered_tile_coords) - (static_cast<sf::Vector2i>(m_render_translation) / tile_size));

		auto active_tile_pos{ m_tile_picker->active_tile() };
		m_level->at(selected_tile.x, selected_tile.y) = aco::tile(active_tile_pos.x * 32.0f, active_tile_pos.y * 32.0f);
		m_tilemap = m_level->get_tilemap();
	}
}

void aco::editor_state::handle_mouse_move_event(const sf::Event::MouseMoveEvent& event)
{
	sf::Vector2i mouse_pixel_position{ sf::Mouse::getPosition(m_app_data.window) };
	m_mouse_world_position = m_app_data.window.mapPixelToCoords(mouse_pixel_position);
	m_hovered_tile_coords = sf::Vector2f{ std::floor(m_mouse_world_position.x / tile_size), std::floor(m_mouse_world_position.y / tile_size) };

	const sf::Vector2i current_mouse_position{ event.x, event.y };
	sf::Vector2i mouse_position_delta{ current_mouse_position - m_mouse_click_origin };
	const auto relative_tile_size{ static_cast<int>(tile_size / std::pow(zoom_factor, m_current_zoom)) };
	mouse_position_delta /= relative_tile_size;

	if (m_mouse_state[sf::Mouse::Button::Left])
	{
		word_render_states.transform.translate(speed * static_cast<sf::Vector2f>(mouse_position_delta));
		auto matrix{ word_render_states.transform.getMatrix() };
		m_render_translation = sf::Vector2f{ matrix[12], matrix[13] };
	}
	else if (m_mouse_state[sf::Mouse::Button::Right])
	{
		m_tile_picker->update_active_tile(mouse_position_delta.x, mouse_position_delta.y);
		sf::Vector2i selected_tile(
			static_cast<sf::Vector2i>(m_hovered_tile_coords) - (static_cast<sf::Vector2i>(m_render_translation) / tile_size));

		auto active_tile_pos{ m_tile_picker->active_tile() };
		m_level->at(selected_tile.x, selected_tile.y) = aco::tile(active_tile_pos.x * 32.0f, active_tile_pos.y * 32.0f);
		m_tilemap = m_level->get_tilemap();
	}

	m_mouse_click_origin += relative_tile_size * mouse_position_delta;

	debug_follower.setPosition(m_hovered_tile_coords * static_cast<float>(tile_size));
}

sf::View aco::editor_state::resize_current_view(sf::Vector2u new_size)
{
	auto view{ m_app_data.window.getView() };
	sf::FloatRect view_rect{
		0.f, 0.f, static_cast<float>(new_size.x), static_cast<float>(new_size.y)
	};
	view.reset(view_rect);

	return view;
}

void aco::editor_state::zoom(int delta)
{
	auto view{ resize_current_view(m_app_data.window.getSize()) };
	m_current_zoom -= delta;
	view.zoom(std::pow(zoom_factor, m_current_zoom));
	m_app_data.window.setView(view);
}

aco::tile_picker::tile_picker(const sf::Texture& tileset, sf::Vector2f tile_size)
	: m_tile_count{ calc_tile_count(tileset, tile_size) }
	, m_data{ std::move(split_tileset(tileset, static_cast<sf::Vector2i>(tile_size))) }
	, m_active_tile{ 0, 0 }
{
}

aco::tile_picker::tile_picker(const sf::Texture& tileset, float tile_size)
	: aco::tile_picker::tile_picker(tileset, { tile_size, tile_size })
{
}

sf::Sprite aco::tile_picker::at(size_t pos_x, size_t pos_y) const
{
	assert(pos_x < m_tile_count.x);
	assert(pos_y < m_tile_count.y);

	return m_data[pos_y * m_tile_count.x + pos_x];
}

sf::Vector2<size_t> aco::tile_picker::tile_count() const
{
	return m_tile_count;
}

size_t aco::tile_picker::width() const
{
	return m_tile_count.x;
}

size_t aco::tile_picker::height() const
{
	return m_tile_count.y;
}

void aco::tile_picker::set_active_tile(size_t pos_x, size_t pos_y)
{
	m_active_tile = sf::Vector2<size_t>{ 
		pos_x % m_tile_count.x, pos_y % m_tile_count.y 
	};
}

void aco::tile_picker::update_active_tile(int delta_x, int delta_y)
{
	m_active_tile = sf::Vector2<size_t>{ 
		(m_active_tile.x + delta_x) % m_tile_count.x, 
		(m_active_tile.y + delta_y) % m_tile_count.y
	};
}

sf::Vector2<size_t> aco::tile_picker::active_tile() const
{
	return m_active_tile;
}

sf::Vector2<size_t> aco::tile_picker::calc_tile_count(const sf::Texture& tileset, sf::Vector2f tile_size)
{
	return sf::Vector2<size_t>{
		static_cast<size_t>(tileset.getSize().x / tile_size.x),
		static_cast<size_t>(tileset.getSize().y / tile_size.y)
	};
}

std::vector<sf::Sprite> aco::tile_picker::split_tileset(const sf::Texture& tileset, sf::Vector2i tile_size)
{
	std::vector<sf::Sprite> tile_picker;

	for (size_t y = 0; y < m_tile_count.y; ++y)
	{
		for (size_t x = 0; x < m_tile_count.x; ++x)
		{
			tile_picker.emplace_back(
				tileset,
				sf::IntRect{ 
					static_cast<int>(x * tile_size.x), static_cast<int>(y * tile_size.y), 
					tile_size.x, tile_size.y 
				}
			);
		}
	}

	return tile_picker;
}

void aco::integer_round(int& num, int step)
{
	int sgn = (num >= 0) ? 1 : -1;
	num = (num + sgn * step / 2) / step;
}
