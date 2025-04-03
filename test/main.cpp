// test/main.cpp
#include <zlibrary/API.hpp>
#include <zlibrary/Types.hpp> // Include the new types header
#include <zlibrary/Utils.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;

// myLogger function remains the same
void myLogger(const std::string& msg, const std::string& module, LogLevel level) {
     std::string levelStr;
     switch (level) {
         case LogLevel::Debug:   levelStr = "DEBUG"; break;
         case LogLevel::Info:    levelStr = "INFO"; break;
         case LogLevel::Warning: levelStr = "WARN"; break;
         case LogLevel::Error:   levelStr = "ERROR"; break;
         default:                levelStr = "?????"; break;
     }
     auto now = std::chrono::system_clock::now();
     auto now_c = std::chrono::system_clock::to_time_t(now);
     std::cout << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
               << " [" << std::setw(5) << levelStr << "] "
               << "[" << std::setw(10) << module << "] "
               << msg << std::endl;
}

bool consoleProgress(cpr::cpr_off_t total, cpr::cpr_off_t downloaded) {
     if (total > 0) {
         float percentage = static_cast<float>(downloaded) / total * 100.0f;
         std::cout << "\r[*] Downloading: " << static_cast<int>(percentage) << "% ("
                   << downloaded / (1024.0 * 1024.0) << "/" << total / (1024.0 * 1024.0) << " MB)" << std::flush;
     } else {
         std::cout << "\r[*] Downloading: " << downloaded / (1024.0 * 1024.0) << " MB (Total size unknown)" << std::flush;
     }
     return true;
}

int main(int argc, char* argv[]) {
    setup_utf8_console();

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " \"<search query>\" [download_dir] [base_url]" << std::endl;
        return 1;
    }

    std::string query = argv[1];
    fs::path downloadDir = (argc > 2) ? fs::path(argv[2]) : fs::current_path() / "zlib_downloads";
    std::string baseUrl = (argc > 3) ? std::string(argv[3]) : ""; // Get base URL from args if provided

    // --- Instantiate API ---
    API api; // Uses default base URL initially

    // Set logger
    api.setLogCallback(myLogger);

    // Set base URL if provided via command line
    if (!baseUrl.empty()) {
        api.setBaseUrl(baseUrl);
    } else {
         // Log the default being used if none provided
         api.log(LogLevel::Info, "Main", "Using default base URL: " + api.getBaseUrl());
    }


    std::cout << "--- Searching for: '" << query << "' ---" << std::endl;
    std::vector<BookSearchResult> searchResults = api.searchBooks(query);

    // ... (rest of the main function remains the same) ...
     if (searchResults.empty()) {
         std::cout << "--- No books found matching the search criteria ---" << std::endl;
         return 0;
     }

     std::cout << "\n--- Found " << searchResults.size() << " result(s) ---" << std::endl;
     const BookSearchResult& firstResult = searchResults[0];
     std::cout << "[*] Processing first result: " << firstResult.title << std::endl;

     std::optional<BookInfo> detailsOpt = api.getBookDetails(firstResult.url);

     if (!detailsOpt) {
         return 1;
     }

     const BookInfo& extractedBook = *detailsOpt;
     printBookInfo(extractedBook);

     if (extractedBook.downloadUrl.empty()) {
         std::cout << "\n[!] No download URL found for this book." << std::endl;
         return 0;
     }

     std::cout << "\n[?] Download this book to '" << downloadDir.string() << "'? (y/N): ";
     std::string userInput;
     std::getline(std::cin, userInput);

     if (userInput == "y" || userInput == "Y") {
         bool success = api.downloadBook(extractedBook, downloadDir, consoleProgress);
         std::cout << std::endl;
         if (success) {
             std::cout << "[+] Download potentially successful." << std::endl;
         } else {
             return 1;
         }
     } else {
         std::cout << "[-] Download skipped by user." << std::endl;
     }

    return 0;
}