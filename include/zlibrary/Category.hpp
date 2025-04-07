//
// Created by Advik on 07-04-2025.
//

#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <zlibrary/Export.hpp>
#include <string>

namespace zlibrary {
    using std::string;

    struct ZLIBRARY_API Category {
        const unsigned int id;
        const string name;
    };
}

#endif //CATEGORY_HPP
