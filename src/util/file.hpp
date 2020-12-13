#include <filesystem>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>

namespace util
{
	const std::string wildcard_pattern{ ".*" };

	std::vector<std::string> list_filenames(
		const std::filesystem::path dir_path, std::string extension_pattern = wildcard_pattern);
	bool exists_not_empty(const std::filesystem::path file_path);
	bool write_to_file(std::string_view content, const std::filesystem::path& file_path, bool overwrite);

	template <typename T>
	bool write_to_file(const T& value, const std::filesystem::path& file_path);
	template <typename T>
	T read_from_file(const std::filesystem::path& file_path, const T& default_value);

	std::string replace_extension(std::string path, const std::string& new_extension);
}

template<typename T>
bool util::write_to_file(const T& value, const std::filesystem::path& file_path)
{
	std::ofstream os{ file_path };
	os << value;

	return os.good();
}
template<typename T>
T util::read_from_file(const std::filesystem::path& file_path, const T& default_value)
{
	std::ifstream is{ file_path };

	if (is.is_open())
	{
		T value;
		is >> value;
		return value;
	}

	return default_value;
}