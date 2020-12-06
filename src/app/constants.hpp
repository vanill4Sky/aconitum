#pragma once

#include <string>

namespace aco
{

const std::string levels_dir{ "assets/levels/" };
const std::string tileset_dir{ "assets/textures/" };
const std::string entities_dir{ "assets/scripts/entities/" };
const std::string scripts_dir{ "assets/scripts/" };
const std::string level_scripts_dir{ scripts_dir + "levels/" };
const std::string level_list_dir{ scripts_dir + "level_list.lua" };
const std::string stubs_file_extension{ "_stubs.lua" };
constexpr float editor_width{ 350.0f };

constexpr float splash_screen_display_time{ 2.0f };
const std::string logo_path{ "assets/textures/logo.png" };

const std::string arial_path{ "assets/fonts/arial.ttf" };
const std::string button_path{ "assets/textures/button.png" };
const std::string manu_background_path{ "assets/textures/menu_background.png" };
const std::string menu_header_path{ "assets/textures/menu_header.png" };

constexpr float game_view_zoom{ 0.5f };
constexpr float stalker_loss_distance{ 10.0f };

}