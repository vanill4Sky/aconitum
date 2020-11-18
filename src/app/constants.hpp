#pragma once

#include <string>

namespace aco
{

const std::string levels_dir{ "assets/levels/" };
const std::string tileset_dir{ "assets/textures/" };
const std::string entities_dir{ "assets/scripts/entities/" };
const std::string scripts_dir{ "assets/scripts/" };
const std::string level_scripts_dir{ scripts_dir + "levels/" };
const std::string stubs_file_extension{ "_stubs.lua" };
constexpr float editor_width{ 350.0f };

}