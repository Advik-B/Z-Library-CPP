// Z-Library/src/Utils.cpp
#include "zlibrary/Utils.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>
#include <system_error>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <clocale>
#endif

#include <zlibrary/Export.hpp>

namespace fs = std::filesystem;

ZLIBRARY_API std::string trim(const std::string& str) {
    auto first_not_space = std::find_if_not(str.begin(), str.end(), [](unsigned char c){
        return std::isspace(c, std::locale());
    });
    if (first_not_space == str.end()) {
        return "";
    }
    auto last_not_space = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c){
        return std::isspace(c, std::locale());
    }).base();
    return std::string(first_not_space, last_not_space);
}

ZLIBRARY_API void setup_utf8_console() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #else
    setlocale(LC_ALL, "");
    #endif
    try {
        #ifdef _WIN32
        std::locale::global(std::locale(".UTF8"));
        #else
        try { std::locale::global(std::locale("en_US.UTF-8")); }
        catch (const std::runtime_error&) {
            try { std::locale::global(std::locale("C.UTF-8")); }
            catch (const std::runtime_error&) {
                try { std::locale::global(std::locale("")); }
                catch (const std::runtime_error& e) {
                    std::cerr << "Warning: Could not set a known UTF-8 locale: " << e.what() << std::endl;
                }
            }
        }
        #endif
        std::cout.imbue(std::locale());
        std::cerr.imbue(std::locale());
    } catch (const std::runtime_error& e) {
        std::cerr << "Warning: Failed to set C++ global locale: " << e.what() << std::endl;
    }
}

ZLIBRARY_API std::string sanitize_filename(std::string filename) {
    filename = std::regex_replace(filename, std::regex(R"([\\/*?:"<>|])"), "");
    filename = std::regex_replace(filename, std::regex(R"(\\.{2,})"), ".");
    filename = std::regex_replace(filename, std::regex(R"(\s{2,})"), " ");
    filename = std::regex_replace(filename, std::regex(R"(^[. ]+|[. ]+$)"), "");
    const size_t max_len = 200;
    if (filename.length() > max_len) {
         size_t last_space = filename.rfind(' ', max_len);
         if (last_space != std::string::npos) {
             filename = filename.substr(0, last_space);
         } else {
             filename = filename.substr(0, max_len);
         }
    }
    if (filename.empty()) {
        filename = "downloaded_book";
    }
    return filename;
}

ZLIBRARY_API std::string url_join(const std::string& base, const std::string& relative) {
    if (base.empty()) return relative;
    if (relative.empty()) return base;
    if (relative.rfind("http://", 0) == 0 || relative.rfind("https://", 0) == 0) {
        return relative;
    }
    std::string result = base;
    while (!result.empty() && result.back() == '/') {
        result.pop_back();
    }
    result += '/';
    size_t start_pos = 0;
    while (start_pos < relative.length() && relative[start_pos] == '/') {
        start_pos++;
    }
    result += relative.substr(start_pos);
    return result;
}