//
// Created by Advik on 03-04-2025.
//

#include "Book.hpp"
#include "Utils.hpp"

#include <iostream>
#include <sstream>
#include <HtmlParser/Parser.hpp>
#include <HtmlParser/Query.hpp>
#include <utf8/core.h>

BookInfo fromHTML(const std::string& htmlContentUtf8) {
    BookInfo bookInfo; // Create instance to populate

    // Validate UTF-8 (optional but recommended)
    if (!utf8::is_valid(htmlContentUtf8)) {
         std::cerr << "Warning: Input HTML string contains invalid UTF-8 sequences." << std::endl;
         // Decide how to handle: return empty BookInfo, throw, or try parsing anyway?
         // For now, let's try parsing anyway, but logging is good.
    }

    // Parse the HTML content
    try {
        HtmlParser::Parser Parser;
        const HtmlParser::DOM DOM = Parser.Parse(htmlContentUtf8);
        const HtmlParser::Query RootQuery(DOM.Root());

        // --- Populate BookInfo ---

        // Title
        if (const auto titleNode = RootQuery.SelectFirst("h1.book-title")) {
            bookInfo.title = trim(titleNode->GetTextContent());
        }

        // Details Column (Author, Rating, etc.)
        const auto detailsColNodes = RootQuery.Select("div.col-sm-9");
        if (!detailsColNodes.empty()) {
            HtmlParser::Query detailsQuery(detailsColNodes[0]);

            // Author
            const auto authorItalicNodes = detailsQuery.Select("i");
            if (!authorItalicNodes.empty()) {
                HtmlParser::Query italicQuery(authorItalicNodes[0]);
                if (const auto authorLinkNode = italicQuery.SelectFirst("a.color1")) {
                    bookInfo.author = trim(authorLinkNode->GetTextContent());
                }
            }

            // Rating & Comments Container
            if(const auto ratingDetailNode = detailsQuery.SelectFirst("div.book-rating-detail")) {
                 HtmlParser::Query ratingContainerQuery(ratingDetailNode);
                 if (const auto ratingNode = ratingContainerQuery.SelectFirst(".book-rating")) {
                    // Store the raw string in the new struct member
                    bookInfo.rating.rawString = trim(ratingNode->GetTextContent());
                    // Attempt to parse the rating string "Interest / Quality"
                    std::stringstream ss(bookInfo.rating.rawString);
                    float qRating = 0.0f;
                    char slash = ' ';
                    // Try to read: float space slash space float
                    if (float iRating = 0.0f; (ss >> iRating) && (ss >> slash) && (slash == '/') && (ss >> qRating)) {
                        // Assign to the new struct members
                        bookInfo.rating.book_rating = iRating;
                        bookInfo.rating.file_quality = qRating;
                    } else {
                        // Log error if parsing failed, using the raw string for context
                        std::cerr << "Warning: Could not parse rating string: " << bookInfo.rating.rawString << std::endl;
                    }
                }
             }
        }

        // Description
        if (const auto descriptionNode = RootQuery.SelectFirst("div#bookDescriptionBox")) {
            bookInfo.description = trim(descriptionNode->GetTextContent());
        }

        // Cover Image URL
        if (const auto coverContainer = RootQuery.SelectFirst("div.details-book-cover-container")) {
            HtmlParser::Query coverQuery(coverContainer);
            // Look for the image within the z-cover web component first
            auto coverImgNode = coverQuery.SelectFirst("z-cover img.image");
             if (!coverImgNode || coverImgNode->GetAttribute("src").empty()) {
                // Fallback to the placeholder image if the main one isn't found directly
                 coverImgNode = coverQuery.SelectFirst("z-cover div.placeholder img.image"); // Adjust if placeholder structure differs
                 if (!coverImgNode || coverImgNode->GetAttribute("src").empty()) {
                      // Final fallback to any image directly in the container
                      coverImgNode = coverQuery.SelectFirst("img.image");
                 }
            }
            if (coverImgNode && !coverImgNode->GetAttribute("src").empty()) {
                 bookInfo.coverImageUrl = coverImgNode->GetAttribute("src");
            }
        }


        // Links (Download/Read Online)
        if (const auto downloadLinkNode = RootQuery.SelectFirst("a.addDownloadedBook")) {
            bookInfo.downloadUrl = downloadLinkNode->GetAttribute("href");
        }
        if (const auto readOnlineLinkNode = RootQuery.SelectFirst("a.reader-link")) {
            bookInfo.readOnlineUrl = readOnlineLinkNode->GetAttribute("href");
        }

        // Details from the bookDetailsBox
        if (const auto bookDetailsBoxNode = RootQuery.SelectFirst("div.bookDetailsBox")) {
            HtmlParser::Query detailsBoxQuery(bookDetailsBoxNode);
            const auto properties = detailsBoxQuery.Select("div.bookProperty");

            for (const auto& propNode : properties) {
                if (propNode->GetAttribute("style").find("display: none") != std::string::npos) continue;
                if (propNode->HasClass("property_language_year")) continue;

                HtmlParser::Query propQuery(propNode);

                std::string labelUtf8 = "N/A";
                if (const auto labelNode = propQuery.SelectFirst("div.property_label")) {
                    labelUtf8 = trim(labelNode->GetTextContent());
                    if (!labelUtf8.empty() && labelUtf8.back() == ':') labelUtf8.pop_back();
                }

                std::string valueUtf8 = "N/A";
                if (const auto valueNode = propQuery.SelectFirst(".property_value")) {
                    HtmlParser::Query valueQuery(valueNode);
                    valueUtf8 = trim(valueNode->GetTextContent());

                    if (labelUtf8 == "Categories") {
                        const auto categoryLinkNodes = valueQuery.Select("a"); // Select ALL links
                        if (!categoryLinkNodes.empty()) {
                             bookInfo.categories.clear(); // Clear default if we find links
                             for(const auto& categoryLinkNode : categoryLinkNodes) {
                                bookInfo.categories.push_back(trim(categoryLinkNode->GetTextContent()));
                             }
                        } else if (!valueUtf8.empty()) {
                             bookInfo.categories.push_back(valueUtf8); // Fallback if no link
                        }
                    } else if (labelUtf8 == "Content Type") bookInfo.contentType = valueUtf8;
                    else if (labelUtf8 == "Volume") bookInfo.volume = valueUtf8;
                    else if (labelUtf8 == "Year") {
                        try { bookInfo.year = std::stoi(valueUtf8); }
                        catch (...) { /* Log or ignore error */ }
                    } else if (labelUtf8 == "Edition") bookInfo.edition = valueUtf8;
                    else if (labelUtf8 == "Publisher") bookInfo.publisher = valueUtf8;
                    else if (labelUtf8 == "Language") bookInfo.language = valueUtf8;
                    else if (labelUtf8 == "Pages") {
                         try { bookInfo.pages = std::stoi(valueUtf8); }
                         catch (...) { /* Log or ignore error */ }
                    } else if (labelUtf8 == "ISBN, ASIN, ISSN") bookInfo.isbn_asin_issn = valueUtf8;
                    else if (labelUtf8 == "Series") bookInfo.series = valueUtf8;
                    else if (labelUtf8 == "File") {
                        bookInfo.rawFileInfoString = valueUtf8;
                        size_t commaPos = valueUtf8.find(',');
                        if (commaPos != std::string::npos) {
                            bookInfo.fileFormat = trim(valueUtf8.substr(0, commaPos));
                            std::string sizePart = trim(valueUtf8.substr(commaPos + 1));
                            std::stringstream ss_size(sizePart);
                            float sizeVal; std::string unit;
                            if (ss_size >> sizeVal >> unit && (unit == "MB" || unit == "mb")) {
                                bookInfo.fileSizeMB = sizeVal;
                            }
                        } else { bookInfo.fileFormat = valueUtf8; }
                    } else if (labelUtf8 == "IPFS") {
                        const auto ipfsSpans = valueQuery.Select("span.z-copy-icon");
                        if(!ipfsSpans.empty()) bookInfo.ipfsCID = trim(ipfsSpans[0]->GetTextContent());
                        if(ipfsSpans.size() > 1) bookInfo.ipfsCIDBlake2b = trim(ipfsSpans[1]->GetTextContent());
                    }
                }
            }
        } else {
             std::cerr << "Warning: Could not find 'div.bookDetailsBox' to extract detailed properties." << std::endl;
        }

    } catch (const std::exception& e) {
        // Catch potential exceptions from the parser (e.g., in strict mode)
        // or from string conversions/parsing (like stoi)
        std::cerr << "Error during HTML processing: " << e.what() << std::endl;
        // Return the partially populated (or empty) bookInfo struct
    }

    return bookInfo;
}

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
    std::cout << "File Size (MB): " << (book.fileSizeMB ? std::to_string(*book.fileSizeMB) : "N/A") << std::endl;
    std::cout << "IPFS CID: " << (book.ipfsCID.empty() ? "N/A" : book.ipfsCID) << std::endl;
    std::cout << "IPFS CID Blake2b: " << (book.ipfsCIDBlake2b.empty() ? "N/A" : book.ipfsCIDBlake2b) << std::endl;
    std::cout << "Cover Image URL: " << (book.coverImageUrl.empty() ? "N/A" : book.coverImageUrl) << std::endl;
    std::cout << "Download URL: " << (book.downloadUrl.empty() ? "N/A" : book.downloadUrl) << std::endl;
    std::cout << "Read Online URL: " << (book.readOnlineUrl.empty() ? "N/A" : book.readOnlineUrl) << std::endl;
    std::cout << "--------------------------" << std::endl;
}