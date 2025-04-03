//
// Created by Advik on 03-04-2025.
//

#ifndef BOOK_HPP
#define BOOK_HPP
#include <optional>
#include <string>
#include <vector>
#include <BookRating.hpp>
#include <zlibrary_export.hpp>

struct ZLIBRARY_API BookInfo {
    std::string title;
    std::string author;
    BookRating rating;
    std::string description;
    std::vector<std::string> categories;
    std::string contentType;
    std::string volume;
    std::optional<int> year;
    std::string edition;
    std::string publisher;
    std::string language;
    std::optional<int> pages;
    std::string isbn_asin_issn;
    std::string series;
    std::string fileFormat;
    std::optional<float> fileSizeMB;
    std::string rawFileInfoString;
    std::string ipfsCID;
    std::string ipfsCIDBlake2b;
    std::string coverImageUrl;
    std::string downloadUrl;
    std::string readOnlineUrl;

    BookInfo() = default;
};


ZLIBRARY_API BookInfo fromHTML(const std::string& htmlContent);
ZLIBRARY_API void printBookInfo(const BookInfo& book);

#endif //BOOK_HPP