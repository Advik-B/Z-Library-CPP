// Z-Library/include/Book.hpp
#ifndef BOOK_HPP
#define BOOK_HPP
#include <optional>
#include <string>
#include <vector>
#include <zlibrary/BookRating.hpp>
#include <zlibrary/Export.hpp>

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

// Utility function for printing, kept separate from core API logic
ZLIBRARY_API void printBookInfo(const BookInfo& book);

#endif //BOOK_HPP