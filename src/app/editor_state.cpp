#include "editor_state.hpp"

#include <algorithm>
#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>

#include "../util/file.hpp"

aco::editor_state::editor_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, m_grid{ sf::Vector2f{ 32.0f, 32.0 }, static_cast<sf::Vector2f>(m_app_data.window.getSize()), sf::Color::Red }
	, m_current_zoom{ 0 }
	, debug_follower{ sf::Vector2f{ 32.0f, 32.0 } }
	, m_brush_mode{ aco::brush_mode::bidirectional }
	, m_current_layer{ static_cast<int>(aco::layer::bottom) }
	, m_level_files_list{ util::list_filenames(levels_dir, "json") }
	, m_current_level_file_idx{ 0 }
{
	ImGui::SFML::Init(app_data.window);

	tileset.loadFromFile("assets/textures/tileset_dungeon_02.png");
	m_level = std::make_unique<aco::level>(tileset, 32.0f);
	m_level->update_tilemap();
	m_tile_picker = std::make_unique<aco::tile_picker>(tileset, 32.0f);
	m_horizontal_bounds_input[1] = m_tile_picker->width();
	m_vertical_bounds_input[1] = m_tile_picker->height();

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
		else if (event.type == sf::Event::MouseButtonPressed && !ImGui::IsAnyWindowHovered())
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

	ImGui::TextWrapped("Current level: %s", m_level->filename().data());
	ImGui::Spacing();

	ImGui::PushID("init_input");
	ImGui::InputTextWithHint("", "new level filename", m_new_level_name.data(), m_new_level_name.size()); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::Button("Init", { -1, 0 }))
	{
		init_level();
	}

	ImGui::PushID("load_input");
	ImGui::Combo("", &m_current_level_file_idx, m_level_files_list); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::Button("Load", { -1, 0 }))
	{
		load_level();
	}

	ImGui::PushID("save_input");
	ImGui::InputTextWithHint("", "this level filename", m_save_level_name.data(), m_save_level_name.size()); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::Button("Save", { -1, 0 }))
	{
		save_level();
	}
	ImGui::Separator();

	for (size_t y = 0; y < m_tile_picker->height(); ++y)
	{
		for (size_t x = 0; x < m_tile_picker->width(); ++x)
		{
			const auto id{ static_cast<int>(y * m_tile_picker->width() + x) };
			ImGui::PushID(id);
			if (const sf::Vector2<size_t> current_tile{ x, y }; 
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

	ImGui::Text("Brush mode:");
	ImGui::RadioButton("Bidirectional", &m_brush_mode, aco::brush_mode::bidirectional);
	ImGui::RadioButton("Horizontal only", &m_brush_mode, aco::brush_mode::horizontal_only);
	ImGui::RadioButton("Vertical only", &m_brush_mode, aco::brush_mode::vertical_only);
	ImGui::RadioButton("Fixed", &m_brush_mode, aco::brush_mode::fixed);
	ImGui::RadioButton("Eraser", &m_brush_mode, aco::brush_mode::eraser);

	ImGui::Spacing();
	ImGui::Checkbox("Add collider", &m_is_adding_collider);

	ImGui::Spacing();
	ImGui::Text("Tile wraping range:");
	ImGui::DragInt2("Horizontal", m_horizontal_bounds_input, 0.1f, 0, m_tile_picker->width() - 1);
	ImGui::DragInt2("Vertical", m_vertical_bounds_input, 0.1f, 0, m_tile_picker->height() - 1);

	ImGui::Spacing();
	ImGui::Text("Level layer selection");
	ImGui::Combo("Current layer", &m_current_layer, "bottom\0top\0\0");
	ImGui::Separator();

	ImGui::Checkbox("Show grid", &m_is_grid_visible);
	if (ImGui::Checkbox("Show tile colliders", &m_is_collider_visible))
	{
		m_level->update_tilemap();
	}
	ImGui::Separator();

	ImGui::Text("Hovered tile coordinates: (%.0f, %.0f)", 
		m_hovered_tile_coords.x, m_hovered_tile_coords.y);
	ImGui::Text("Render translation (tiles): (%i, %i)",
		m_level->render_translation().x, m_level->render_translation().y);

	ImGui::End();

	ImGui::EndFrame();

	m_grid.set_visible(m_is_grid_visible);
	validate_bounds_input();
}

void aco::editor_state::draw()
{
	m_app_data.window.clear();
	m_level->draw(m_app_data.window, m_is_collider_visible);
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
	sf::Vector2i mouse_pos{ event.x, event.y };
	m_origin_tile_coords = calc_tile_coordinates(mouse_pos, m_level->tile_size());
	m_prev_tile_delta = sf::Vector2f{ 0.0f, 0.0f };

	if (event.button == sf::Mouse::Right)
	{
		update_tile(mouse_pos, m_level->tile_size());
	}
	
}

void aco::editor_state::handle_mouse_move_event(const sf::Event::MouseMoveEvent& event)
{
	sf::Vector2i mouse_pos{ event.x, event.y };
	m_hovered_tile_coords = calc_tile_coordinates(mouse_pos, m_level->tile_size());
	const sf::Vector2f new_tile_delta{ m_hovered_tile_coords - m_origin_tile_coords };

	if (m_mouse_state[sf::Mouse::Button::Left] && new_tile_delta != m_prev_tile_delta)
	{
		m_level->move(-m_prev_tile_delta);
		m_level->move(new_tile_delta);
	}
	else if (m_mouse_state[sf::Mouse::Button::Right] && new_tile_delta != m_prev_tile_delta)
	{
		const auto step = static_cast<sf::Vector2i>(new_tile_delta - m_prev_tile_delta);
		m_tile_picker->update_active_tile(step.x, step.y, static_cast<aco::brush_mode>(m_brush_mode));

		update_tile(mouse_pos, m_level->tile_size());
	}

	m_prev_tile_delta = new_tile_delta;

	debug_follower.setPosition(m_hovered_tile_coords * static_cast<float>(m_level->tile_size()));
}

void aco::editor_state::validate_bounds_input()
{
	const auto validate_bound = [](auto& input, auto limit) {
		limit = (limit > 0) ? limit - 1 : 0;
		input[0] = std::min(input[0], static_cast<int>(limit));
		input[1] = std::min(input[1], static_cast<int>(limit));
		input[1] = std::max(input[0], input[1]);
	};
	validate_bound(m_horizontal_bounds_input, m_tile_picker->width());
	validate_bound(m_vertical_bounds_input, m_tile_picker->height());
}

sf::View aco::editor_state::resize_current_view(sf::Vector2u new_size)
{
	auto view{ m_app_data.window.getView() };
	const sf::FloatRect view_rect{
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

sf::Vector2f aco::editor_state::calc_tile_coordinates(sf::Vector2i mouse_position, float tile_size) const
{
	auto mouse_world_position = m_app_data.window.mapPixelToCoords(mouse_position);

	return { 
		std::floor(mouse_world_position.x / tile_size), 
		std::floor(mouse_world_position.y / tile_size) 
	};
}

sf::Vector2i aco::editor_state::calc_tile_world_coordinates(sf::Vector2i mouse_position, float tile_size)
{
	auto tile_coordinates{ 
		static_cast<sf::Vector2i>(calc_tile_coordinates(mouse_position, tile_size)) 
	};

	return tile_coordinates - m_level->render_translation();
}

void aco::editor_state::update_tile(sf::Vector2i mouse_position, float tile_size)
{
	const auto selected_tile{ calc_tile_world_coordinates(mouse_position, tile_size) };

	auto active_tile_pos{ m_tile_picker->active_tile() };
	if (auto& tile{ m_level->at(static_cast<aco::layer>(m_current_layer), selected_tile.x, selected_tile.y) };
		static_cast<aco::brush_mode>(m_brush_mode) == aco::eraser)
	{
		tile = aco::tile();
	}
	else
	{
		tile = aco::tile(active_tile_pos.x * 32.0f, active_tile_pos.y * 32.0f, m_is_adding_collider);
	}
	m_level->update_tilemap();
}

void aco::editor_state::init_level()
{
	std::string new_level_filename{ m_new_level_name.data() };
	aco::level new_level{ tileset, 32.0f };
	new_level.set_filename(new_level_filename);
	new_level.write_to_file(levels_dir);
	m_level_files_list = util::list_filenames(levels_dir, "json");

	m_new_level_name[0] = '\0';
}

void aco::editor_state::load_level()
{
	if (!m_level_files_list.empty())
	{
		const auto& filename{ m_level_files_list[m_current_level_file_idx] };
		m_level->read_from_file(levels_dir + filename);
		std::copy(filename.begin(), filename.end(), m_save_level_name.begin());
		m_save_level_name[std::min(filename.size(), m_save_level_name.size() - 1)] = '\0';
	}
}

void aco::editor_state::save_level()
{
	m_level->set_filename(m_save_level_name.data());
	m_level->write_to_file(levels_dir);
	m_level_files_list = util::list_filenames(levels_dir, "json");
}

bool ImGui::Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return Combo(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size());
}

bool ImGui::ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size());
}