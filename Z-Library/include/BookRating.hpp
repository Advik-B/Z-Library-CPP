//
// Created by Advik on 03-04-2025.
//

#ifndef BOOKRATING_HPP
#define BOOKRATING_HPP
#include <iostream>
#include <optional>
#include <string>
#include <zlibrary_export.hpp>

struct ZLIBRARY_API BookRating {
    std::optional<float> book_rating; // Interest score (e.g., 5.0)
    std::optional<float> file_quality;  // File quality score (e.g., 3.0)
    std::string rawString;        // Original combined string (e.g., "5.0 / 3.0")

    // Default constructor initializes optionals to nullopt
    BookRating() = default;

    // A helper function, similar to __str__ in Python, to print the rating
    [[nodiscard]] std::string toString() const {
        return "Book Rating: " + (book_rating ? std::to_string(*book_rating) : "N/A") +
               " / File Quality: " + (file_quality ? std::to_string(*file_quality) : "N/A");
    }
};

#endif //BOOKRATING_HPP
