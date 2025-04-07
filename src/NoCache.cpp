//
// Created by Advik on 07-04-2025.
//

#include <zlibrary/NoCache.hpp>

inline void zlibrary::NoCache::set(const string & key, const string& value) {}
inline std::string zlibrary::NoCache::get(const string & key) { return ""; }
inline bool zlibrary::NoCache::get(const string &key, string &result_string) { return false; }
inline void zlibrary::NoCache::purge() {}
