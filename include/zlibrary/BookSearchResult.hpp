// Z-Library/include/BookSearchResult.hpp
#ifndef BOOKSEARCHRESULT_HPP
#define BOOKSEARCHRESULT_HPP

#include <string>
#include <vector>
#include <optional>
#include "Export.hpp"

struct ZLIBRARY_API BookSearchResult {
    std::string title;
    std::string url;
    std::optional<std::string> book_id;
    std::optional<std::string> author;
    std::optional<std::string> year;
    std::optional<std::string> publisher;
    std::optional<std::string> extension;
    std::optional<std::string> size;
    std::optional<std::string> language;

    BookSearchResult() = default;
};

#endif // BOOKSEARCHRESULT_HPP