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
        	string base_url;
            ZCacheTypePtr cache;

		public:
      		cpr::Session session;
            void setBaseURL(const string& url);
            string getBaseURL() const;

    };
}

#endif //ZLIBRARYAPI_HPP
