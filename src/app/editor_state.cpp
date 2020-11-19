#include "editor_state.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <sstream>

#include "../util/file.hpp"

aco::editor_state::editor_state(aco::app_data& app_data)
	: m_app_data{ app_data }
	, m_grid{ sf::Vector2f{ 32.0f, 32.0 }, static_cast<sf::Vector2f>(m_app_data.window.getSize()), sf::Color::Red }
	, m_current_zoom{ 0 }
	, debug_follower{ sf::Vector2f{ 32.0f, 32.0 } }
	, m_brush_mode{ aco::brush_mode::bidirectional }
	, m_current_layer{ static_cast<int>(aco::layer::bottom) }
	, m_level_files_list{ util::list_filenames(levels_dir, "json") }
	, m_tileset_files_list{ util::list_filenames(tileset_dir) }
	, m_current_level_file_idx{ 0 }
	, m_current_tileset_file_idx{ 0 }

	, m_current_entity_file_idx{ 0 }
{
	m_level = std::make_unique<aco::level>(32.0f);
	m_level->update_tilemap();
	m_horizontal_bounds_input[1] = static_cast<int>(m_tile_picker.width());
	m_vertical_bounds_input[1] = static_cast<int>(m_tile_picker.height());

	get_available_templates(scripts_dir + "entities.lua");
	load_template();

	debug_follower.setFillColor(sf::Color(0, 255, 0, 63));
}

aco::editor_state::~editor_state()
{
	ImGui::EndFrame();
	ImGui::SFML::Shutdown();
}

void aco::editor_state::init()
{
	ImGui::SFML::Init(m_app_data.window);
	m_level->reset_to_origin();
	zoom(m_current_zoom);
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
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_app_data.state_manager.pop_state();
				break;
			}
		}
	}

}

void aco::editor_state::update(float dt)
{
	ImGui::SFML::Update(m_app_data.window, sf::seconds(dt));

	ImGui::ShowTestWindow();

	ImGui::SetNextWindowPos({ m_app_data.window.getSize().x - editor_width, 0.0f });
	ImGui::SetNextWindowSize({ editor_width,  static_cast<float>(m_app_data.window.getSize().y) });
	ImGui::Begin("Aconitum editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::BeginTabBar("editor_tab_bar", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Level editor"))
		{
			update_level_editor_tab();
			ImGui::EndTabItem();
			m_is_level_editor_visible = true;
		}
		if (ImGui::BeginTabItem("Entity placer"))
		{
			update_entity_placer_tab();
			ImGui::EndTabItem();
			m_is_level_editor_visible = false;
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::EndFrame();

	m_grid.set_visible(m_is_grid_visible);
	validate_bounds_input();
}

void aco::editor_state::draw()
{
	m_app_data.window.clear();
	m_level->draw(m_app_data.window, m_is_collider_visible);
	m_app_data.window.draw(debug_follower);
	for (const auto& stub : m_stubs)
	{
		m_app_data.window.draw(stub.shape, m_level->level_render_states());
	}
	m_app_data.window.draw(m_grid);
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
	const auto translated_mouse_pos =
		m_app_data.window.mapPixelToCoords(mouse_pos) - m_level->render_translation();
	m_prev_mouse_pos = translated_mouse_pos;


	if (event.button == sf::Mouse::Right)
	{
		if (m_is_level_editor_visible && m_tile_picker.is_tileset_loaded())
		{
			m_origin_tile_coords = calc_tile_coordinates(mouse_pos, m_level->tile_size());
			m_prev_tile_delta = sf::Vector2f{ 0.0f, 0.0f };

			update_tile(mouse_pos, m_level->tile_size());
		}
		else if (!m_is_level_editor_visible)
		{
			sf::Sprite sprite{ m_miniature };
			const auto sprite_bounds{ sprite.getGlobalBounds() };
			sprite.setPosition(translated_mouse_pos - sf::Vector2f{ sprite_bounds.width, sprite_bounds.height } / 2.0f);
			m_stubs.emplace_back(aco::entity_stub{ current_entity_name(), sprite });
		}
	}
	else if (event.button == sf::Mouse::Left)
	{
		highlight_entity(m_selected_entity_idx, false);
		m_selected_entity_idx = -1;
		for (auto it{ m_stubs.rbegin() }; it != m_stubs.rend(); ++it)
		{
			if (it->shape.getGlobalBounds().contains(translated_mouse_pos))
			{
				m_selected_entity_idx = static_cast<int>(std::distance(it, m_stubs.rend())) - 1;
				highlight_entity(m_selected_entity_idx, true);
				break;
			}

		}
	}
}

void aco::editor_state::handle_mouse_move_event(const sf::Event::MouseMoveEvent& event)
{
	sf::Vector2i mouse_pos{ event.x, event.y };
	m_hovered_tile_coords = calc_tile_coordinates(mouse_pos, m_level->tile_size());

	const sf::Vector2f new_tile_delta{ m_hovered_tile_coords - m_origin_tile_coords };

	if (m_mouse_state[sf::Mouse::Button::Middle] && new_tile_delta != m_prev_tile_delta)
	{
		m_level->move(-m_prev_tile_delta);
		m_level->move(new_tile_delta);
	}
	else if (m_is_level_editor_visible && m_tile_picker.is_tileset_loaded()
		&& m_mouse_state[sf::Mouse::Button::Right] && new_tile_delta != m_prev_tile_delta)
	{
		const auto step = static_cast<sf::Vector2i>(new_tile_delta - m_prev_tile_delta);
		m_tile_picker.update_active_tile(step.x, step.y, static_cast<aco::brush_mode>(m_brush_mode));

		update_tile(mouse_pos, m_level->tile_size());
	}

	m_prev_tile_delta = new_tile_delta;

	if (m_mouse_state[sf::Mouse::Button::Left])
	{
		const auto translated_mouse_pos = 
			m_app_data.window.mapPixelToCoords(mouse_pos) - m_level->render_translation();

		if (m_selected_entity_idx >= 0)
		{
			m_stubs[m_selected_entity_idx].shape.move(translated_mouse_pos - m_prev_mouse_pos);
		}

		m_prev_mouse_pos = translated_mouse_pos;
	}

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
	validate_bound(m_horizontal_bounds_input, m_tile_picker.width());
	validate_bound(m_vertical_bounds_input, m_tile_picker.height());
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

	return tile_coordinates - m_level->render_translation_tiles();
}

void aco::editor_state::update_level_editor_tab()
{
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

	ImGui::TextWrapped("Tileset: %s", m_level->tileset_filename().data());
	ImGui::PushID("set_tileset_input");
	ImGui::Combo("", &m_current_tileset_file_idx, m_tileset_files_list); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::Button("Set", { -1, 0 }))
	{
		if (m_level->load_tileset(tileset_dir + m_tileset_files_list[m_current_tileset_file_idx]))
		{
			m_tile_picker = aco::tile_picker(m_level->tileset(), m_level->tile_size());
		}
	}
	ImGui::Separator();

	if (ImGui::Button("Origin", { 0, 0 }))
	{
		m_level->reset_to_origin();
		zoom(m_current_zoom);
	}
	ImGui::SameLine();
	if (ImGui::Button("Optimize", { 0, 0 }))
	{
		m_level->optimize_size();
	}
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Level size: (%llu, %llu)", m_level->width(), m_level->height());
	ImGui::Separator();

	for (size_t y = 0; y < m_tile_picker.height(); ++y)
	{
		for (size_t x = 0; x < m_tile_picker.width(); ++x)
		{
			const auto id{ static_cast<int>(y * m_tile_picker.width() + x) };
			ImGui::PushID(id);
			if (const sf::Vector2<size_t> current_tile{ x, y };
				m_tile_picker.active_tile() == current_tile)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
				ImGui::PushStyleColor(ImGuiCol_Border, { 1.0f, 0.0f, 0.0f, 1.0f });
			}
			else
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleColor(ImGuiCol_Border, { 0.0f, 0.0f, 0.0f, 1.0f });
			}
			if (ImGui::ImageButton(m_tile_picker.at(x, y), 1))
			{
				m_tile_picker.set_active_tile(x, y);
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
	ImGui::DragInt2("Horizontal", m_horizontal_bounds_input, 0.1f, 0, m_tile_picker.width() - 1);
	ImGui::DragInt2("Vertical", m_vertical_bounds_input, 0.1f, 0, m_tile_picker.height() - 1);

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
		m_level->render_translation_tiles().x, m_level->render_translation_tiles().y);
}

void aco::editor_state::update_entity_placer_tab()
{
	if (ImGui::Combo("Templates", &m_current_entity_file_idx, m_entity_names_list))
	{
		load_template();
	}
	ImGui::Text("Sprite component:");
	const auto max_width{ ImGui::GetWindowContentRegionWidth() };
	ImGui::Image(m_miniature, { max_width, max_width }, sf::Color::White, sf::Color::White);
	ImGui::Separator();

	ImGui::Text("File:");
	if (ImGui::Button("Load stubs"))
	{
		load_stubs(get_stubs_filename());
	}
	ImGui::SameLine();
	if (ImGui::Button("Save stubs"))
	{
		save_stubs(get_stubs_filename());
	}
	ImGui::Spacing();

	ImGui::Text("List of entities:");
	if (ImGui::Button("/\\", { 40.0f, 0.0f }) && m_selected_entity_idx > 0)
	{
		move_selected_stub(static_cast<size_t>(m_selected_entity_idx) - 1);
	}
	ImGui::SameLine();
	if (ImGui::Button("\\/", { 40.0f, 0.0f }) && m_selected_entity_idx < (m_stubs.size() - 1))
	{
		move_selected_stub(static_cast<size_t>(m_selected_entity_idx) + 1);
	}
	ImGui::SameLine();
	if (ImGui::Button("/|\\", { 40.0f, 0.0f }) && m_selected_entity_idx > 0)
	{
		move_selected_stub(0);
	}
	ImGui::SameLine();
	if (ImGui::Button("\\|/", { 40.0f, 0.0f }) && m_selected_entity_idx < (m_stubs.size() - 1))
	{
		move_selected_stub(static_cast<int>(m_stubs.size()) - 1);
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 110.0f);
	if (ImGui::Button("Delete entity", { 100.0f, 0.0f }) && m_selected_entity_idx > -1)
	{
		m_stubs.erase(m_stubs.begin() + m_selected_entity_idx);
		--m_selected_entity_idx;
		highlight_entity(m_selected_entity_idx, true);
	}
	ImGui::SetNextItemWidth(-1.0f);
	int previous_entity = m_selected_entity_idx;
	ImGui::ListBox("", &m_selected_entity_idx, m_stubs);
	if (ImGui::IsItemEdited())
	{
		highlight_entity(previous_entity, false);
		highlight_entity(m_selected_entity_idx, true);
	}
}

void aco::editor_state::update_tile(sf::Vector2i mouse_position, float tile_size)
{
	const auto selected_tile{ calc_tile_world_coordinates(mouse_position, tile_size) };

	auto active_tile_pos{ m_tile_picker.active_tile() };
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
	aco::level new_level{ 32.0f };
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

		m_tile_picker = aco::tile_picker(m_level->tileset(), m_level->tile_size());
	}
}

void aco::editor_state::save_level()
{
	m_level->set_filename(m_save_level_name.data());
	m_level->write_to_file(levels_dir);
	m_level_files_list = util::list_filenames(levels_dir, "json");
}

void aco::editor_state::get_available_templates(const std::string& index_filename)
{
	m_app_data.lua.script_file(index_filename);
	m_entities = m_app_data.lua["entities_templates"];

	for (const auto& pair : m_entities)
	{
		sol::object key = pair.first;
		m_entity_names_list.emplace_back(key.as<std::string>());
	}

	m_current_entity_file_idx = 0;
}

void aco::editor_state::load_template()
{
	sol::optional<std::string> tex_path_opt = 
		m_entities[m_entity_names_list[m_current_entity_file_idx]]["sprite"]["tex_path"];
	if (tex_path_opt != sol::nullopt && util::exists_not_empty(tex_path_opt.value()))
	{
		const auto tex_path = tex_path_opt.value();
		m_app_data.textures.load(tex_path);
		m_miniature.setTexture(m_app_data.textures.get(tex_path), true);

		sol::optional<sf::Vector2f> frame_size_opt =
			m_entities[m_entity_names_list[m_current_entity_file_idx]]["animation"]["frame_size"];
		if (frame_size_opt != sol::nullopt)
		{
			m_miniature.setTextureRect({ sf::Vector2i{0, 0}, static_cast<sf::Vector2i>(frame_size_opt.value()) });
		}
	}
}

sf::Sprite aco::editor_state::load_template(std::string_view name)
{
	sf::Sprite sprite;
	sol::optional<sol::table> entity_template_opt = m_entities[name.data()];
	if (entity_template_opt == sol::nullopt)
	{
		spdlog::error("Entity template \"{}\" is not defined.", name);
		return sprite;
	}
	const auto& entity_template{ entity_template_opt.value() };
	sol::optional<std::string> tex_path_opt = entity_template["sprite"]["tex_path"];
	if (tex_path_opt != sol::nullopt && util::exists_not_empty(tex_path_opt.value()))
	{
		const auto& tex_path{ tex_path_opt.value() };
		m_app_data.textures.load(tex_path);
		sprite.setTexture(m_app_data.textures.get(tex_path), true);

		sol::optional<sf::Vector2f> frame_size_opt = entity_template["animation"]["frame_size"];
		if (frame_size_opt != sol::nullopt)
		{
			sprite.setTextureRect({ sf::Vector2i{0, 0}, static_cast<sf::Vector2i>(frame_size_opt.value()) });
		}
	}
	else
	{
		spdlog::warn("Entity \"{}\" has no sprite component.", name);
	}

	return sprite;
}

std::string aco::editor_state::current_entity_name() const
{
	return m_entity_names_list[m_current_entity_file_idx];
}

void aco::editor_state::highlight_entity(int index, bool is_highlighted)
{
	if (index < 0)
	{
		return;
	}

	if (is_highlighted)
	{
		m_stubs[index].shape.setOutlineColor(sf::Color::Green);
	}
	else
	{
		m_stubs[index].shape.setOutlineColor(sf::Color::Transparent);
	}
}

std::string aco::editor_state::get_stubs_filename() const
{
	const auto& level_filename{ m_level->filename() };
	const auto last_dot{ level_filename.rfind('.') };
	return level_filename.substr(0, last_dot) + stubs_file_extension;
}

void aco::editor_state::save_stubs(const std::string& stubs_filename)
{
	std::stringstream ss;
	ss << R"(
-- BEGINING OF GENERATED CODE

local stubs = {
)";
	for (size_t i{ 0 }; i < m_stubs.size(); ++i)
	{
		const auto& stub{ m_stubs[i] };
		const auto& pos = stub.shape.getPosition();
		ss << "\t" << stub.id
			<< " = { idx = " << i
			<< ", name = \"" << stub.name
			<< "\", position = ex_vector2f:new(" << pos.x
			<< ", " << pos.y
			<< ") },\n";
	}
	ss << R"(}

function get_stubs()
	return stubs
end

return get_stubs

-- END OF GENERATED CODE)";

	const auto path{ level_scripts_dir + stubs_filename };
	if (util::write_to_file(ss.str(), path, true))
	{
		spdlog::info("Succesfully written entities stubs to file: {}", path);
	}
	else
	{
		spdlog::error("Failed to write entities stubs to file: {}", path);
	}
}

void aco::editor_state::load_stubs(const std::string& stubs_filename)
{
	m_stubs = std::vector<entity_stub>();
	entity_stub::reset_counter();

	m_app_data.lua.script_file(level_scripts_dir + stubs_filename);
	const sol::table stubs = m_app_data.lua["get_stubs"]();
	for (const auto& pair : stubs)
	{
		sol::table value = pair.second.as<sol::table>();

		std::string id = pair.first.as<std::string>();
		std::string name = value["name"];
		sf::Vector2f position = value["position"];
		size_t idx = value["idx"];
		
		sf::Sprite sprite = load_template(name);
		sprite.setPosition(position);

		m_stubs.emplace_back(id, name, sprite, idx);
	}

	std::sort(m_stubs.begin(), m_stubs.end(), [](const auto& a, const auto& b) { return a.idx < b.idx; });
}

void aco::editor_state::move_selected_stub(const size_t new_pos)
{
	auto selected_elem_it{ m_stubs.begin() + m_selected_entity_idx };
	auto selected_elem{ std::move(*selected_elem_it) };
	m_stubs.erase(selected_elem_it);
	m_stubs.insert(m_stubs.begin() + new_pos, selected_elem);
	m_selected_entity_idx = static_cast<int>(new_pos);
	highlight_entity(m_selected_entity_idx, true);
}

bool ImGui::Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return Combo(label, currIndex, vector_getter,
		static_cast<void*>(&values), static_cast<int>(values.size()));
}

bool ImGui::ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&values), static_cast<int>(values.size()));
}

bool ImGui::ListBox(const char* label, int* currIndex, std::vector<aco::entity_stub>& values)
{
	const auto items_count{ static_cast<int>(values.size()) };
	if (values.empty()) { return false; }
	return ListBox(label, currIndex, vector_of_stubs_getter,
		static_cast<void*>(&values), items_count, items_count);
}

std::string aco::entity_stub::generate_id(const std::string& name)
{
	std::string id{ name + "_" + std::to_string(entity_stub::entity_count[name]) };

	return id;
}

void aco::entity_stub::reset_counter()
{
	entity_count.clear();
}

aco::entity_stub::entity_stub(std::string id, std::string name, const sf::Sprite& sprite, size_t idx)
	: id{ std::move(id) }
	, name{ std::move(name) }
	, idx{ idx }
{
	entity_stub::entity_count.try_emplace(this->name, 0);
	++entity_stub::entity_count[this->name];

	shape.setTexture(sprite.getTexture());
	shape.setTextureRect(sprite.getTextureRect());

	const auto sprite_bounds{ sprite.getGlobalBounds() };
	shape.setSize({ sprite_bounds.width, sprite_bounds.height });
	shape.setPosition(sprite.getPosition());

	shape.setOutlineColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);
}

aco::entity_stub::entity_stub(std::string name, const sf::Sprite& sprite)
	: entity_stub(generate_id(name), name, sprite, 0)
{ }

std::unordered_map<std::string, int> aco::entity_stub::entity_count;
