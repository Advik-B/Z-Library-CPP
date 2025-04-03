// Z-Library/include/BookRating.hpp
#ifndef BOOKRATING_HPP
#define BOOKRATING_HPP
#include <iostream>
#include <optional>
#include <string>
#include <zlibrary/Export.hpp>

struct ZLIBRARY_API BookRating {
    std::optional<float> book_rating;
    std::optional<float> file_quality;
    std::string rawString;

    BookRating() = default;

    [[nodiscard]] std::string toString() const {
        return "Book Rating: " + (book_rating ? std::to_string(*book_rating) : "N/A") +
               " / File Quality: " + (file_quality ? std::to_string(*file_quality) : "N/A");
    }
};

#endif //BOOKRATING_HPP