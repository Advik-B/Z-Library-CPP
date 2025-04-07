//
// Created by Advik on 07-04-2025.
//

#include <cpr/cpr.h>
#include <memory>
#include <zlibrary/NoCache.hpp>
#include <zlibrary/Utils/UserAgent.hpp>
#include <zlibrary/ZLibraryAPI.hpp>

using namespace std;

std::string zlibrary::ZLibraryAPI::url_join(const std::string &base, const std::string &path) {
        if (base.empty()) return path;
        if (path.empty())
            return base;

        const bool baseEndsWithSlash = base.back() == '/';
        const bool pathStartsWithSlash = path.front() == '/';

        if (baseEndsWithSlash && pathStartsWithSlash) {
            return base + path.substr(1); // Remove extra slash
        }
        if (!baseEndsWithSlash && !pathStartsWithSlash) {
            return base + "/" + path; // Add missing slash
        }
        return base + path; // Already correctly formatted
}

void zlibrary::ZLibraryAPI::setCache(const CachePtr &new_cache) {
    // this->cache->purge(); // Uncomment if you want to purge the old cache before setting a new one
    this->cache = new_cache;
}
void zlibrary::ZLibraryAPI::removeCache() { this->cache = std::make_shared<NoCache>(); }

void zlibrary::ZLibraryAPI::setBaseURL(const string &new_base_url) { this->base_url = new_base_url; }

cpr::Response zlibrary::ZLibraryAPI::get(const string &path) {
    // Url join the base_url and path
    const string full_url = url_join(base_url, path);
    // Set the URL in the session
    session.SetUrl(full_url);
    // Perform the GET request
    cpr::Response response = session.Get();
    // Check if the response is successful
    if (response.status_code != 200) {
        // Handle error
        throw std::runtime_error("Failed to fetch data from ZLibrary API");
    }
    return response;
}
cpr::Response zlibrary::ZLibraryAPI::post(const string &path, const cpr::Payload &payload) {
    throw std::runtime_error("Not implemented");
}
zlibrary::ZLibraryAPI::ZLibraryAPI() {
    this->base_url = "https://z-library.sk/";
    session.SetUrl(base_url);
    session.SetHeader(
        {{"User-Agent", utils::generate_useragent()},
            {"Accept-Language", "en-US,en;q=0.9"},
            {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/"
                            "apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
            {"Connection", "keep-alive"},
            {"Upgrade-Insecure-Requests", "1"},
            {"DNT", "1"},
            {"Sec-Fetch-Dest", "document"}, // Common browser headers
            {"Sec-Fetch-Mode", "navigate"},
            {"Sec-Fetch-Site", "same-origin"}, // Adjust if needed, but good default
            {"Referer", base_url}
        }
    );
    removeCache();
}

zlibrary::CachePtr zlibrary::ZLibraryAPI::getCache() { return this->cache; }
