//
// Created by Advik on 07-04-2025.
//

#ifndef ZLIBRARYAPI_HPP
#define ZLIBRARYAPI_HPP

#include <zlibrary/Export.hpp>
#include <zlibrary/ZCacheType.hpp>
#include <cpr/cpr.h>
#include <string>

namespace zlibrary {
	using std::string;
    class ZLIBRARY_API ZLibraryAPI {
      	private:
            CachePtr cache;
            string base_url;

		public:
      		cpr::Session session;
            void setCache(CachePtr new_cache);
            void removeCache();
            void setBaseURL(const string& new_base_url);
            cpr::Response get(const string &path);
            cpr::Response post(const string &path, const cpr::Payload &payload);

            ZLibraryAPI(cpr::Session &session);
            ZLibraryAPI();
    };
}

#endif //ZLIBRARYAPI_HPP
