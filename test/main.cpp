#include <Utils.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <Book.hpp>


namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    setup_utf8_console();
    // --- Check if the HTML file exists ---
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_html_file>" << std::endl;
        return 1;
    }
    fs::path htmlFilePath = argv[1];

    // --- Load HTML from file (as before) ---
    // fs::path htmlFilePath = fs::current_path() / "book_page.html";
    if (!fs::exists(htmlFilePath)) { std::cerr << "HTML file not found.\n"; return 1; }
    std::ifstream htmlFile(htmlFilePath, std::ios::binary);
    if (!htmlFile.is_open()) { std::cerr << "Failed to open HTML file.\n"; return 1; }
    std::string htmlContentUtf8((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    htmlFile.close();

    // --- Call the function to get the struct ---
    const BookInfo extractedBook = fromHTML(htmlContentUtf8);

    // --- Print the results from the struct ---
    printBookInfo(extractedBook);

    return 0;
}