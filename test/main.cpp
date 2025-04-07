#include <zlibrary/Enums.hpp>
#include <iostream>
using namespace std;

int main() {
    constexpr auto lang = zlibrary::Language::ARABIC;
    const string langStr = toString(lang);
    cout << "Lang: " + langStr << "\n";

}