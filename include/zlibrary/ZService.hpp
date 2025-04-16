//
// Created by Advik on 16-04-2025.
//

#ifndef ZSERVICE_HPP
#define ZSERVICE_HPP

#include <zlibrary/ZLibraryAPI.hpp>
#include <zlibrary/Book.hpp>
#include <zlibrary/Export.hpp>
#include <zlibrary/Enums.hpp>
#include <vector>
#include <string>
#include <optional>


namespace zlibrary {
    using namespace std;

    struct ZLIBRARY_API ZSearchQuery {
        const string query;
        const optional<unsigned int> year_from;
        const optional<unsigned int> year_to;
        const optional<vector<Language>> languages;
        const optional<vector<Extension>> extensions;
        const optional<vector<TypeOfContent>> typeofcontent;

        ZSearchQuery fromQuery(const string &query);
    };

    class ZLIBRARY_API ZService {
    public:
        ZService(ZLibraryAPI *api);

        vector<Book> searchBooks(const ZSearchQuery &query, const OrderOptions &order = OrderOptions::POPULAR);
        Book getBook(const string &url);
        ~ZService();
    private:
        ZLibraryAPI *api;
    };

};

#endif //ZSERVICE_HPP
