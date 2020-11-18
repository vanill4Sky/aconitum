#include <filesystem>
#include <vector>
#include <string>
#include <string_view>

namespace util
{
	const std::string wildcard_pattern{ ".*" };

	std::vector<std::string> list_filenames(
		const std::filesystem::path dir_path, std::string extension_pattern = wildcard_pattern);
	bool exists_not_empty(const std::filesystem::path file_path);
	bool write_to_file(std::string_view content, const std::filesystem::path& file_path, bool overwrite);

	std::string replace_extension(std::string path, const std::string& new_extension);
}