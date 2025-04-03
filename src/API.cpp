// Z-Library/src/API.cpp
#include "zlibrary/API.hpp"
#include "zlibrary/Enums.hpp" // Include enums
#include "zlibrary/Utils.hpp"
#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <HtmlParser/Parser.hpp>
#include <HtmlParser/Query.hpp>
#include <utf8/core.h>
#include <cpr/cpr.h>

namespace fs = std::filesystem;
// Use the namespace alias for clarity
namespace zlenum = zlib;

// --- Default Base URL (Static or Constexpr if needed outside) ---
const std::string DEFAULT_BASE_URL = "https://z-library.sk";

// --- Constructors ---
API::API() : base_url_(DEFAULT_BASE_URL) {
    setup_session_defaults();
}

API::API(std::string base_url) : base_url_(std::move(base_url)) {
    if (base_url_.empty()) {
        base_url_ = DEFAULT_BASE_URL;
        // Cannot log here yet as callback might not be set
    }
    setup_session_defaults();
}

API::API(LogCallback logger)
    : base_url_(DEFAULT_BASE_URL), log_callback_(std::move(logger)) {
     setup_session_defaults();
}

API::API(std::string base_url, LogCallback logger)
    : base_url_(std::move(base_url)), log_callback_(std::move(logger)) {
    if (base_url_.empty()) {
        base_url_ = DEFAULT_BASE_URL;
        log(LogLevel::Warning, "Config", "Empty base URL provided, using default: " + base_url_);
    }
    setup_session_defaults();
}

// --- Configuration Methods ---
void API::setLogCallback(LogCallback logger) {
    log_callback_ = std::move(logger);
}

void API::setBaseUrl(const std::string& base_url) {
    if (base_url.empty()) {
        log(LogLevel::Warning, "Config", "Attempted to set empty base URL, keeping current: " + base_url_);
        return;
    }
    base_url_ = base_url;
    setup_session_defaults();
    log(LogLevel::Info, "Config", "Base URL set to: " + base_url_);
}

const std::string& API::getBaseUrl() const {
    return base_url_;
}


// --- Private Log Helper ---
void API::log(LogLevel level, const std::string& module, const std::string& msg) {
    if (log_callback_) {
        try {
             log_callback_(msg, module, level);
        } catch (const std::exception& e) {
             fprintf(stderr, "API: Exception in log callback: %s\n", e.what());
        } catch (...) {
             fprintf(stderr, "API: Unknown exception in log callback.\n");
        }
    }
}

// Setup session defaults
void API::setup_session_defaults() {
    session_.SetHeader({
        {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"},
        {"Accept-Language", "en-US,en;q=0.9"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8"},
        {"Connection", "keep-alive"},
        {"DNT", "1"},
        {"Upgrade-Insecure-Requests", "1"},
        {"Referer", base_url_}
    });
    session_.SetRedirect(cpr::Redirect{true});
    session_.SetTimeout(cpr::Timeout{30000});
    session_.SetConnectTimeout(cpr::ConnectTimeout{15000});
    session_.SetVerifySsl(cpr::VerifySsl{false});
}

// --- Updated searchBooks implementation ---
std::vector<BookSearchResult> API::searchBooks(
    const std::string& query,
    const std::optional<zlenum::OrderOptions>& order,
    bool exact_match,
    const std::optional<int>& year_from,
    const std::optional<int>& year_to,
    const std::vector<zlenum::Language>& languages,
    const std::vector<zlenum::Extension>& extensions
) {
    std::string search_path = "/s/";
    cpr::Url search_url = cpr::Url{url_join(base_url_, search_path)};
    cpr::Parameters params;
    params.Add({"q", query});

    if (order) params.Add({"order", zlenum::toString(*order)});
    if (exact_match) params.Add({"e", "1"});
    if (year_from) params.Add({"yearFrom", std::to_string(*year_from)});
    if (year_to) params.Add({"yearTo", std::to_string(*year_to)});

    // Note: Assuming comma-separated for now. Adjust if needed.
    if (!languages.empty()) {
        params.Add({"languages", zlenum::join(languages)});
    }
    if (!extensions.empty()) {
        params.Add({"extensions", zlenum::join(extensions)});
    }

    session_.SetUrl(search_url);
    session_.SetParameters(params);

    log(LogLevel::Info, "Search", "Searching Z-Lib: " + session_.GetFullRequestUrl());
    cpr::Response response = session_.Get();
    std::vector<BookSearchResult> results_list;

    if (response.status_code >= 200 && response.status_code < 300) {
        log(LogLevel::Info, "Search", "Search request successful (Status: " + std::to_string(response.status_code) + ")");
        log(LogLevel::Debug, "Search", "Final Search URL: " + response.url.str());
        session_.SetHeader({{"Referer", response.url.str()}});

        try {
             HtmlParser::Parser parser;
             const HtmlParser::DOM dom = parser.Parse(response.text);
             const auto resultItems = dom.Root()->Select("div#searchResultBox div.book-item.resItemBoxBooks"); // Might need adjustment

             log(LogLevel::Info, "Search", "Found " + std::to_string(resultItems.size()) + " potential book items on search results page.");

             for (const auto& item : resultItems) {
                 HtmlParser::Query itemQuery(item);
                 const auto bookCard = itemQuery.SelectFirst("z-bookcard"); // Might need adjustment
                 if (!bookCard) {
                     log(LogLevel::Warning, "Search", "Skipping item, could not find z-bookcard.");
                     continue;
                 }
                  std::string relative_url = bookCard->GetAttribute("href");
                  std::string book_id = bookCard->GetAttribute("id");
                  std::string title = "Title N/A";

                  const auto titleSlot = itemQuery.SelectFirst("div[slot='title']"); // Might need adjustment
                  if (titleSlot) {
                      title = trim(titleSlot->GetTextContent());
                  }

                  // TODO: Extract other optional fields from search result card

                  if (!relative_url.empty()) {
                      BookSearchResult result;
                      result.title = title;
                      result.url = url_join(base_url_, relative_url);
                      if (!book_id.empty()) result.book_id = book_id;
                      results_list.push_back(result);
                  } else {
                      log(LogLevel::Warning, "Search", "Skipping book card, missing href attribute: " + title);
                  }
             }
        } catch (const std::exception& e) {
             log(LogLevel::Error, "Search", "Error parsing search results HTML: " + std::string(e.what()));
        }
    } else {
        std::stringstream ss;
        ss << "Search request failed (Status: " << response.status_code << "): " << response.error.message;
        log(LogLevel::Error, "Search", ss.str());
    }
    return results_list;
}

std::optional<BookInfo> API::getBookDetails(const std::string& bookUrl) {
    session_.SetUrl(cpr::Url{bookUrl});
    session_.SetParameters({});
    session_.SetPayload({});
    session_.SetMultipart({});

    log(LogLevel::Info, "Details", "Fetching details from: " + bookUrl);
    cpr::Response response = session_.Get();

    if (response.status_code >= 200 && response.status_code < 300) {
         log(LogLevel::Info, "Details", "Detail page request successful (Status: " + std::to_string(response.status_code) + ")");
         session_.SetHeader({{"Referer", bookUrl}});
        return this->fromHTML(response.text);
    } else {
         std::stringstream ss;
         ss << "Detail page request failed (Status: " << response.status_code << "): " << response.error.message;
         log(LogLevel::Error, "Details", ss.str());
        return std::nullopt;
    }
}

bool API::downloadBook(const BookInfo& bookInfo, const fs::path& downloadDir,
                               std::function<bool(cpr::cpr_off_t total, cpr::cpr_off_t downloaded)> progressCallback) {
    if (bookInfo.downloadUrl.empty()) {
        log(LogLevel::Error, "Download", "No primary download URL found in book details.");
        return false;
    }
    try {
        fs::create_directories(downloadDir);
    } catch (const std::exception& e) {
         log(LogLevel::Error, "Download", "Failed to create download directory '" + downloadDir.string() + "': " + e.what());
        return false;
    }
    std::string base_filename = (bookInfo.author.empty() ? "Unknown Author" : bookInfo.author) + " - " + (bookInfo.title.empty() ? "Unknown Title" : bookInfo.title);
    std::string sanitized_base = sanitize_filename(base_filename);
    std::string file_extension = bookInfo.fileFormat.empty() ? "epub" : bookInfo.fileFormat;
    file_extension = file_extension.substr(0, file_extension.find(','));
    file_extension = trim(file_extension);
    if (file_extension.empty()) file_extension = "epub";
    fs::path filepath = downloadDir / (sanitized_base + "." + file_extension);

    log(LogLevel::Info, "Download", "Attempting download from: " + bookInfo.downloadUrl);
    log(LogLevel::Info, "Download", "Saving to: " + filepath.string());

    std::ofstream fileStream(filepath, std::ios::binary);
    if (!fileStream) {
         log(LogLevel::Error, "Download", "Failed to open file for writing: " + filepath.string());
        return false;
    }
    session_.SetUrl(cpr::Url{bookInfo.downloadUrl});
    session_.SetRedirect(cpr::Redirect{true});

    cpr::Response response;
    if (progressCallback) {
         response = session_.Download(fileStream, cpr::Progress{progressCallback});
    } else {
         response = session_.Download(fileStream);
    }
     fileStream.close();

     if (response.status_code >= 200 && response.status_code < 300) {
         if (response.header["content-type"].find("text/html") != std::string::npos) {
              log(LogLevel::Warning, "Download", "Received an HTML page. Login/Captcha might be required. File saved, but might not be the book: " + filepath.string());
         } else {
              log(LogLevel::Info, "Download", "Download complete: " + filepath.string());
         }
         return true;
     } else {
         std::stringstream ss;
         ss << "Download failed (Status: " << response.status_code << "): " << response.error.message;
         log(LogLevel::Error, "Download", ss.str());
         try {
             fs::remove(filepath);
         } catch (const std::exception& e) {
              log(LogLevel::Error, "Download", "Failed to remove incomplete download file '" + filepath.string() + "': " + e.what());
         }
         return false;
     }
}

// --- fromHTML Implementation (Private member of API) ---
BookInfo API::fromHTML(const std::string& htmlContentUtf8) {
     BookInfo bookInfo;
     if (!utf8::is_valid(htmlContentUtf8)) {
          log(LogLevel::Warning, "Parse", "Input HTML string contains invalid UTF-8 sequences.");
     }
     try {
         HtmlParser::Parser parser;
         const HtmlParser::DOM dom = parser.Parse(htmlContentUtf8);
         const HtmlParser::Query RootQuery(dom.Root());

         if (const auto titleNode = RootQuery.SelectFirst("h1.book-title")) { bookInfo.title = trim(titleNode->GetTextContent()); }

         const auto detailsColNodes = RootQuery.Select("div.col-sm-9");
         if (!detailsColNodes.empty()) {
             HtmlParser::Query detailsQuery(detailsColNodes[0]);
             const auto authorItalicNodes = detailsQuery.Select("i");
             if (!authorItalicNodes.empty()) {
                 HtmlParser::Query italicQuery(authorItalicNodes[0]);
                 if (const auto authorLinkNode = italicQuery.SelectFirst("a.color1")) { bookInfo.author = trim(authorLinkNode->GetTextContent()); }
             }
             if(const auto ratingDetailNode = detailsQuery.SelectFirst("div.book-rating-detail")) {
                  HtmlParser::Query ratingContainerQuery(ratingDetailNode);
                  if (const auto ratingNode = ratingContainerQuery.SelectFirst(".book-rating")) {
                     bookInfo.rating.rawString = trim(ratingNode->GetTextContent());
                     std::stringstream ss(bookInfo.rating.rawString);
                     float qRating = 0.0f; char slash = ' ';
                     if (float iRating = 0.0f; (ss >> iRating) && (ss >> slash) && (slash == '/') && (ss >> qRating)) {
                         bookInfo.rating.book_rating = iRating; bookInfo.rating.file_quality = qRating;
                     } else { log(LogLevel::Warning, "Parse", "Could not parse rating string: " + bookInfo.rating.rawString); }
                 }
              }
         }
         if (const auto descriptionNode = RootQuery.SelectFirst("div#bookDescriptionBox")) { bookInfo.description = trim(descriptionNode->GetTextContent()); }

         if (const auto coverContainer = RootQuery.SelectFirst("div.details-book-cover-container")) {
             HtmlParser::Query coverQuery(coverContainer);
             auto coverImgNode = coverQuery.SelectFirst("z-cover img.image");
              if (!coverImgNode || coverImgNode->GetAttribute("src").empty()) {
                  coverImgNode = coverQuery.SelectFirst("z-cover div.placeholder img.image");
                  if (!coverImgNode || coverImgNode->GetAttribute("src").empty()) { coverImgNode = coverQuery.SelectFirst("img.image"); }
              }
              if (coverImgNode && !coverImgNode->GetAttribute("src").empty()) { bookInfo.coverImageUrl = coverImgNode->GetAttribute("src"); }
         }
         if (const auto downloadLinkNode = RootQuery.SelectFirst("a.addDownloadedBook")) {
             bookInfo.downloadUrl = downloadLinkNode->GetAttribute("href");
             if (!bookInfo.downloadUrl.empty() && bookInfo.downloadUrl.front() == '/') { bookInfo.downloadUrl = url_join(base_url_, bookInfo.downloadUrl); }
         }
         if (const auto readOnlineLinkNode = RootQuery.SelectFirst("a.reader-link")) {
             bookInfo.readOnlineUrl = readOnlineLinkNode->GetAttribute("href");
             if (!bookInfo.readOnlineUrl.empty() && bookInfo.readOnlineUrl.front() == '/') { bookInfo.readOnlineUrl = url_join(base_url_, bookInfo.readOnlineUrl); }
         }
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
                          const auto categoryLinkNodes = valueQuery.Select("a");
                          if (!categoryLinkNodes.empty()) {
                               bookInfo.categories.clear();
                               for(const auto& categoryLinkNode : categoryLinkNodes) { bookInfo.categories.push_back(trim(categoryLinkNode->GetTextContent())); }
                          } else if (!valueUtf8.empty()) { bookInfo.categories.push_back(valueUtf8); }
                     } else if (labelUtf8 == "Content Type") { bookInfo.contentType = valueUtf8; }
                     else if (labelUtf8 == "Volume") { bookInfo.volume = valueUtf8; }
                     else if (labelUtf8 == "Year") { try { bookInfo.year = std::stoi(valueUtf8); } catch (...) { log(LogLevel::Warning, "Parse", "Failed to parse year: " + valueUtf8); } }
                     else if (labelUtf8 == "Edition") { bookInfo.edition = valueUtf8; }
                     else if (labelUtf8 == "Publisher") { bookInfo.publisher = valueUtf8; }
                     else if (labelUtf8 == "Language") { bookInfo.language = valueUtf8; }
                     else if (labelUtf8 == "Pages") { try { bookInfo.pages = std::stoi(valueUtf8); } catch (...) { log(LogLevel::Warning, "Parse", "Failed to parse pages: " + valueUtf8); } }
                     else if (labelUtf8 == "ISBN, ASIN, ISSN") { bookInfo.isbn_asin_issn = valueUtf8; }
                     else if (labelUtf8 == "Series") { bookInfo.series = valueUtf8; }
                     else if (labelUtf8 == "File") {
                          bookInfo.rawFileInfoString = valueUtf8;
                          size_t commaPos = valueUtf8.find(',');
                          if (commaPos != std::string::npos) {
                              bookInfo.fileFormat = trim(valueUtf8.substr(0, commaPos));
                              std::string sizePart = trim(valueUtf8.substr(commaPos + 1));
                              std::stringstream ss_size(sizePart);
                              float sizeVal; std::string unit;
                              if (ss_size >> sizeVal >> unit && (unit == "MB" || unit == "mb")) { bookInfo.fileSizeMB = sizeVal; }
                              else { log(LogLevel::Warning, "Parse", "Failed to parse file size: " + sizePart); }
                          } else { bookInfo.fileFormat = valueUtf8; }
                     } else if (labelUtf8 == "IPFS") {
                          const auto ipfsSpans = valueQuery.Select("span.z-copy-icon");
                          if(!ipfsSpans.empty()) bookInfo.ipfsCID = trim(ipfsSpans[0]->GetTextContent());
                          if(ipfsSpans.size() > 1) bookInfo.ipfsCIDBlake2b = trim(ipfsSpans[1]->GetTextContent());
                     }
                 }
             }
         } else { log(LogLevel::Warning, "Parse", "Could not find 'div.bookDetailsBox' to extract detailed properties."); }
     } catch (const std::exception& e) { log(LogLevel::Error, "Parse", "Exception during HTML processing: " + std::string(e.what())); }
     return bookInfo;
 }