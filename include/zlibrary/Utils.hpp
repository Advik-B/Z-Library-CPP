// Z-Library/include/Utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "Export.hpp"
#include <filesystem>

namespace fs = std::filesystem;

ZLIBRARY_API std::string trim(const std::string& str);
ZLIBRARY_API void setup_utf8_console();
ZLIBRARY_API std::string sanitize_filename(std::string filename);
ZLIBRARY_API std::string url_join(const std::string& base, const std::string& relative);

#endif //UTILS_HPP