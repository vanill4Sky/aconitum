#include "file.hpp"

#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

std::vector<std::string> util::list_filenames(
    const std::filesystem::path dir_path, std::string extension_pattern)
{
    const auto str_to_lower = [](std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::tolower(c); });
    };
    std::vector<std::string> file_list;

    str_to_lower(extension_pattern);
    if (extension_pattern.size() > 0 && extension_pattern[0] != '.')
    {
        extension_pattern = "." + extension_pattern;
    }

    for (const auto& entry : fs::directory_iterator(dir_path))
    {
        if (entry.is_regular_file())
        {
            auto extension{ entry.path().extension().string() };
            str_to_lower(extension);

            if (extension_pattern == util::wildcard_pattern 
                || extension_pattern == extension)
            {
                file_list.emplace_back(entry.path().filename().string());
            }
        }
    }

    return file_list;
}

bool util::exists_not_empty(const std::filesystem::path file_path)
{
    return fs::exists(file_path) && fs::is_regular_file(file_path) 
        && (fs::file_size(file_path) != 0);
}
