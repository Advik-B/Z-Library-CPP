//
// Created by Advik on 07-04-2025.
//

#ifndef NOCACHE_HPP
#define NOCACHE_HPP

#include <zlibrary/CacheType.hpp>
#include <zlibrary/Export.hpp>

namespace zlibrary {

    /*
    Does not cache anything anywhere.
     */
    class ZLIBRARY_API NoCache : public ICache {
         void set(const string & key, const string& value) override;
         string get(const string & key) override;
         bool get(const string &key, string &result_string) override;
         void purge() override;
    };

}

#endif //NOCACHE_HPP
