//
// Created by Advik on 07-04-2025.
//

#include <stdexcept>
#include <unordered_map>
#include <zlibrary/Enums.hpp>

namespace zlibrary {
    std::string toString(const Extension e) {
        // Simple linear search is fine for small enums if map lookup isn't needed often
        for(const auto&[fst, snd] : extension_map_data) {
            if (fst == e) {
                return snd;
            }
        }
        throw std::logic_error("Invalid Extension enum value"); // Should not happen
    }

    Extension stringToExtension(const std::string &s) {
        // Create the map on first call (thread-safe in C++11+)
        static const std::unordered_map<std::string, Extension> lookup_map = [] {
            std::unordered_map<std::string, Extension> map;
            for (const auto& pair : extension_map_data) {
                map[pair.second] = pair.first;
            }
            return map;
        }();

        const auto it = lookup_map.find(s);
        if (it == lookup_map.end()) {
            throw std::runtime_error("Invalid string for Extension enum: " + s);
        }
        return it->second;
    }

    std::string toString(OrderOptions o) {
        for(const auto&[fst, snd] : orderoptions_map_data) {
            if (fst == o) {
                return snd;
            }
        }
        throw std::logic_error("Invalid OrderOptions enum value"); // Should not happen
    }

    OrderOptions stringToOrderOptions(const std::string &s) {
        static const std::unordered_map<std::string, OrderOptions> lookup_map = [] {
            std::unordered_map<std::string, OrderOptions> map;
            for (const auto&[fst, snd] : orderoptions_map_data) {
                map[snd] = fst;
            }
            return map;
        }();

        const auto it = lookup_map.find(s);
        if (it == lookup_map.end()) {
            throw std::runtime_error("Invalid string for OrderOptions enum: " + s);
        }
        return it->second;
    }

    std::string toString(Language lang) {
        // For very large enums, a map lookup might be preferred even for enum->string,
        // but linear search is simple.
        for(const auto&[fst, snd] : language_map_data) {
            if (fst == lang) {
                return snd;
            }
        }
        throw std::logic_error("Invalid Language enum value"); // Should not happen
    }

    Language stringToLanguage(const std::string &s) {
        // Create the map on first call (thread-safe in C++11+)
        static const std::unordered_map<std::string, Language> lookup_map = [] {
            std::unordered_map<std::string, Language> map;
            for (const auto&[fst, snd] : language_map_data) {
                map[snd] = fst;
            }
            return map;
        }();

        const auto it = lookup_map.find(s);
        if (it == lookup_map.end()) {
            throw std::runtime_error("Invalid string for Language enum: " + s);
        }
        return it->second;
    }
}