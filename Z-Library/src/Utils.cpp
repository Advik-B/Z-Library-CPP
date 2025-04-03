//
// Created by Advik on 03-04-2025.
//

#include "Utils.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>

// --- Platform-specific includes and functions for UTF-8 console output ---
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <clocale>
#endif

#include <zlibrary_export.hpp>

// Helper function to trim leading/trailing whitespace (using locale)
std::string trim(const std::string& str) {
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


void setup_utf8_console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_ALL, ""); // Use environment's locale setting
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
// --- End of platform-specific setup ---