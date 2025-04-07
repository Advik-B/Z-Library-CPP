//
// Created by Advik on 07-04-2025.
//

#include <zlibrary/ZLibraryAPI.hpp>
#include <zlibrary/NoCache.hpp>
#include <cpr/cpr.h>


zlibrary::ZLibraryAPI::ZLibraryAPI() {
    this->base_url = "https://z-library.sk/";
    session.SetUrl(base_url);
}