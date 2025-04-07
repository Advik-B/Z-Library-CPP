#include <zlibrary/Utils.hpp>
#include <filesystem>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>

namespace fs = std::filesystem;

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

std::string sanitize_filename(std::string filename) {
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

std::string url_join(const std::string& base, const std::string& relative) {
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