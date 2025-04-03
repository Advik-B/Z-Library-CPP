// Z-Library/include/ZLibraryTypes.hpp
#ifndef ZLIBRARYTYPES_HPP
#define ZLIBRARYTYPES_HPP

#include <string>
#include <functional> // Needed for std::function
#include "Export.hpp" // Include export macros

// Define Log Levels
enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3
};

// Define the callback function type
using LogCallback = ZLIBRARY_API std::function<void(const std::string& msg, const std::string& module, LogLevel level)>;

#endif // ZLIBRARYTYPES_HPP