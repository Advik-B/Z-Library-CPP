// Z-Library/include/API.hpp
#ifndef ZLIBRARYAPI_HPP
#define ZLIBRARYAPI_HPP

#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <filesystem>
#include "Export.hpp"
#include "Types.hpp"     // Include the new types header
#include "Enums.hpp"     // Include the new enums header
#include "BookSearchResult.hpp"
#include "Book.hpp"
#include <cpr/cpr.h>

namespace fs = std::filesystem;

class ZLIBRARY_API API {
private:
    std::string base_url_;
    cpr::Session session_;
    LogCallback log_callback_;

    void setup_session_defaults();
    BookInfo fromHTML(const std::string& htmlContentUtf8);
    void log(LogLevel level, const std::string& module, const std::string& msg);

public:
    // --- Constructors ---
    API();
    explicit API(std::string base_url);
    explicit API(LogCallback logger);
    API(std::string base_url, LogCallback logger);

    // --- Configuration Methods ---
    void setLogCallback(LogCallback logger);
    void setBaseUrl(const std::string& base_url);
    [[nodiscard]] const std::string& getBaseUrl() const;

    // --- Updated Public API Methods ---
    std::vector<BookSearchResult> searchBooks(
        const std::string& query,
        const std::optional<zlib::OrderOptions>& order = std::nullopt, // Use enum type
        bool exact_match = false,
        const std::optional<int>& year_from = std::nullopt,
        const std::optional<int>& year_to = std::nullopt,
        const std::vector<zlib::Language>& languages = {},    // Use enum type
        const std::vector<zlib::Extension>& extensions = {} // Use enum type
    );

    std::optional<BookInfo> getBookDetails(const std::string& bookUrl);

    bool downloadBook(const BookInfo& bookInfo, const fs::path& downloadDir,
                      std::function<bool(cpr::cpr_off_t total, cpr::cpr_off_t downloaded)> progressCallback = nullptr);
};

#endif // ZLIBRARYAPI_HPP