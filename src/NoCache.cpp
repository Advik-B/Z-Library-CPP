//
// Created by Advik on 07-04-2025.
//

#include <zlibrary/NoCache.hpp>

inline void zlibrary::NoCache::set(const string &, const string&) {}
inline std::string zlibrary::NoCache::get(const string &) { return ""; }
inline bool zlibrary::NoCache::get(const string &, string &) { return false; }
inline void zlibrary::NoCache::purge() {}
