#include <filesystem>
#include <vector>
#include <string>

namespace util
{
	const std::string wildcard_pattern{ ".*" };

	std::vector<std::string> list_filenames(
		const std::filesystem::path dir_path, std::string extension_pattern = wildcard_pattern);
	bool exists_not_empty(const std::filesystem::path file_path);
}