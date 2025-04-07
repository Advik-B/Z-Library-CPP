//
// Created by Advik on 07-04-2025.
//

#ifndef BOOK_HPP
#define BOOK_HPP

#include <zlibrary/Export.hpp>
#include <zlibrary/Category.hpp>
#include <zlibrary/Enums.hpp>
#include <zlibrary/Rating.hpp>
#include <string>

namespace zlibrary {
    using std::string;

    struct ZLIBRARY_API Book {
        const string title;
        const string author;
        const Rating rating;
        const string cover_url;
        const string description;
        const Category category;
        const Language language;
        const unsigned int filesize;
        const Extension extension;
        const string download_url;
    };
}

#endif //BOOK_HPP
