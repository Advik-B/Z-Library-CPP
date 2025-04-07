#include <zlibrary/Enums.hpp>
#include <zlibrary/ZLibraryAPI.hpp>
#include <iostream>
using namespace std;

int main() {
    constexpr auto lang = zlibrary::Language::ARABIC;
    const string langStr = toString(lang);
    cout << "Lang: " + langStr << "\n";
    auto api = zlibrary::ZLibraryAPI();
    api.getCache()->purge();
    cout << "Yo";
    try {
        api.get("x");
    } catch (std::exception &e) {
        cout << "Error: " << e.what() << "\n";
    }
    return 0;
}