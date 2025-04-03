// Z-Library/src/Book.cpp
#include "zlibrary/Book.hpp"
#include "zlibrary/Utils.hpp"

#include <iostream>
#include <sstream>
#include <iomanip> // For printing floats nicely

// Utility function for printing BookInfo
void printBookInfo(const BookInfo& book) {
    std::cout << "--- Book Information ---" << std::endl;
    std::cout << "Title: " << (book.title.empty() ? "N/A" : book.title) << std::endl;
    std::cout << "Author: " << (book.author.empty() ? "N/A" : book.author) << std::endl;
    std::cout << "Rating: " << book.rating.toString() << std::endl;
    std::cout << "Description: " << (book.description.empty() ? "N/A" : book.description) << std::endl;
    std::cout << "Categories: ";
    if (book.categories.empty()) {
        std::cout << "N/A";
    } else {
        for (size_t i = 0; i < book.categories.size(); ++i) {
            std::cout << book.categories[i] << (i == book.categories.size() - 1 ? "" : ", ");
        }
    }
    std::cout << std::endl;
    std::cout << "Content Type: " << (book.contentType.empty() ? "N/A" : book.contentType) << std::endl;
    std::cout << "Volume: " << (book.volume.empty() ? "N/A" : book.volume) << std::endl;
    std::cout << "Year: " << (book.year ? std::to_string(*book.year) : "N/A") << std::endl;
    std::cout << "Edition: " << (book.edition.empty() ? "N/A" : book.edition) << std::endl;
    std::cout << "Publisher: " << (book.publisher.empty() ? "N/A" : book.publisher) << std::endl;
    std::cout << "Language: " << (book.language.empty() ? "N/A" : book.language) << std::endl;
    std::cout << "Pages: " << (book.pages ? std::to_string(*book.pages) : "N/A") << std::endl;
    std::cout << "ISBN/ASIN/ISSN: " << (book.isbn_asin_issn.empty() ? "N/A" : book.isbn_asin_issn) << std::endl;
    std::cout << "Series: " << (book.series.empty() ? "N/A" : book.series) << std::endl;
    std::cout << "Raw File Info: " << (book.rawFileInfoString.empty() ? "N/A" : book.rawFileInfoString) << std::endl;
    std::cout << "File Format: " << (book.fileFormat.empty() ? "N/A" : book.fileFormat) << std::endl;
    std::cout << "File Size (MB): ";
    if (book.fileSizeMB) {
        std::cout << std::fixed << std::setprecision(2) << *book.fileSizeMB;
    } else {
        std::cout << "N/A";
    }
    std::cout << std::endl;
    std::cout << "IPFS CID: " << (book.ipfsCID.empty() ? "N/A" : book.ipfsCID) << std::endl;
    std::cout << "IPFS CID Blake2b: " << (book.ipfsCIDBlake2b.empty() ? "N/A" : book.ipfsCIDBlake2b) << std::endl;
    std::cout << "Cover Image URL: " << (book.coverImageUrl.empty() ? "N/A" : book.coverImageUrl) << std::endl;
    std::cout << "Download URL: " << (book.downloadUrl.empty() ? "N/A" : book.downloadUrl) << std::endl;
    std::cout << "Read Online URL: " << (book.readOnlineUrl.empty() ? "N/A" : book.readOnlineUrl) << std::endl;
    std::cout << "--------------------------" << std::endl;
}