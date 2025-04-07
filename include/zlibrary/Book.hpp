//
// Created by Advik on 07-04-2025.
//

#ifndef BOOK_HPP
#define BOOK_HPP

#include <zlibrary/Export.hpp>
#include <zlibrary/Category.hpp>
#include <string>

namespace zlibrary {
    using std::string;

    struct ZLIBRARY_API Book {
        const string title;
        const string author;
        const string cover_url;
        const string description;
        const Category category;
        const string isbn;
        

    };
}

#endif //BOOK_HPP
