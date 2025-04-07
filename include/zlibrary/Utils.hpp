// Z-Library/include/Utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "Export.hpp"
#include <filesystem>

namespace fs = std::filesystem;

std::string trim(const std::string& str);
std::string sanitize_filename(std::string filename);
std::string url_join(const std::string& base, const std::string& relative);

#endif //UTILS_HPP