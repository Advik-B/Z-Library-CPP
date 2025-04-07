//
// Created by Advik on 07-04-2025.
//

#ifndef ZCACHETYPE_HPP
#define ZCACHETYPE_HPP

#include <string>
#include <memory>
#include <zlibrary/Export.hpp>

namespace zlibrary {
    using std::string;
    using std::shared_ptr;

    class ZLIBRARY_API ZCacheType {
    public:
        virtual ~ZCacheType() = default;

        virtual void set(const string & key, const string& value) = 0;
        virtual string get(const string & key) = 0;
        virtual bool get(const string &key, string &result_string) = 0;
        virtual void purge() = 0;
    };

    ZLIBRARY_API typedef shared_ptr<ZCacheType> CachePtr;
}

#endif //ZCACHETYPE_HPP
