//
// Created by Advik on 07-04-2025.
//

#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <zlibrary/Export.hpp>
#include <string>
#include <unordered_set>

namespace zlibrary {
    using std::string;

    struct ZLIBRARY_API Category {
        const unsigned int id;
        const string name;
    };

    static ZLIBRARY_API std::unordered_set<Category> loaded_categories; // Will populate as the library gets used

}

#endif //CATEGORY_HPP
