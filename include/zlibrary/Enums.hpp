//
// Created by Advik on 07-04-2025.
//

#ifndef ENUMS_HPP
#define ENUMS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <utility>
#include <zlibrary/Export.hpp>

namespace zlibrary {

//-----------------------------------------------------
// Extension Enum
//-----------------------------------------------------
enum class Extension {
    TXT,
    PDF,
    FB2,
    EPUB,
    LIT,
    MOBI,
    RTF,
    DJV,
    DJVU,
    AZW,
    AZW3
};

// Helper data for Extension mapping (string representation matches enum name)
static const std::vector<std::pair<Extension, std::string>> extension_map_data = {
    {Extension::TXT, "TXT"}, {Extension::PDF, "PDF"},   {Extension::FB2, "FB2"},
    {Extension::EPUB, "EPUB"}, {Extension::LIT, "LIT"},   {Extension::MOBI, "MOBI"},
    {Extension::RTF, "RTF"}, {Extension::DJV, "DJV"},   {Extension::DJVU, "DJVU"},
    {Extension::AZW, "AZW"}, {Extension::AZW3, "AZW3"}
};

// Function to get the string representation of Extension
ZLIBRARY_API inline std::string toString(Extension e) {
    // Simple linear search is fine for small enums if map lookup isn't needed often
    for(const auto& pair : extension_map_data) {
        if (pair.first == e) {
            return pair.second;
        }
    }
    throw std::logic_error("Invalid Extension enum value"); // Should not happen
}

// Function to get Extension from string
ZLIBRARY_API inline Extension stringToExtension(const std::string& s) {
    // Create the map on first call (thread-safe in C++11+)
    static const std::unordered_map<std::string, Extension> lookup_map = [] {
        std::unordered_map<std::string, Extension> map;
        for (const auto& pair : extension_map_data) {
            map[pair.second] = pair.first;
        }
        return map;
    }();

    auto it = lookup_map.find(s);
    if (it == lookup_map.end()) {
        throw std::runtime_error("Invalid string for Extension enum: " + s);
    }
    return it->second;
}


//-----------------------------------------------------
// OrderOptions Enum
//-----------------------------------------------------
enum class OrderOptions {
    POPULAR,
    NEWEST,
    RECENT
};

// Helper data for OrderOptions mapping (string representation is custom)
static const std::vector<std::pair<OrderOptions, std::string>> orderoptions_map_data = {
    {OrderOptions::POPULAR, "popular"},
    {OrderOptions::NEWEST, "date_created"},
    {OrderOptions::RECENT, "date_updated"}
};

// Function to get the string representation of OrderOptions
ZLIBRARY_API inline std::string toString(OrderOptions o) {
    for(const auto& pair : orderoptions_map_data) {
        if (pair.first == o) {
            return pair.second;
        }
    }
    throw std::logic_error("Invalid OrderOptions enum value"); // Should not happen
}

// Function to get OrderOptions from string
ZLIBRARY_API inline OrderOptions stringToOrderOptions(const std::string& s) {
    static const std::unordered_map<std::string, OrderOptions> lookup_map = [] {
        std::unordered_map<std::string, OrderOptions> map;
        for (const auto& pair : orderoptions_map_data) {
            map[pair.second] = pair.first;
        }
        return map;
    }();

    auto it = lookup_map.find(s);
    if (it == lookup_map.end()) {
        throw std::runtime_error("Invalid string for OrderOptions enum: " + s);
    }
    return it->second;
}


//-----------------------------------------------------
// Language Enum
//-----------------------------------------------------
enum class Language {
    ARABIC, ARMENIAN, AZERBAIJANI, BENGALI, CHINESE, DUTCH, ENGLISH, FRENCH,
    GEORGIAN, GERMAN, GREEK, HINDI, INDONESIAN, ITALIAN, JAPANESE, KOREAN,
    MALAYSIAN, PASHTO, POLISH, PORTUGUESE, RUSSIAN, SERBIAN, SPANISH, TELUGU,
    THAI, TURKISH, UKRAINIAN, URDU, VIETNAMESE, ABKHAZIAN, AFAR, AFRIKAANS,
    AKAN, YORUBA, ZHUANG, ZULU, ALBANIAN, AMHARIC, ARAGONESE, ASSAMESE,
    AVARIC, AVESTAN, AYMARA, BAMBARA, BASHKIR, BASQUE, BELARUSIAN, BERBER,
    BISLAMA, BOSNIAN, BRETON, BULGARIAN, BURMESE, CATALAN, CENTRAL_KHMER,
    CHAMORRO, CHECHEN, CHICHEWA, CHURCH_SLAVIC, CHUVASH, CORNISH, CORSICAN,
    CREE, CRIMEAN, CROATIAN, CZECH, DANISH, DIVEHI, DZONGKHA, ESPERANTO,
    ESTONIAN, EWE, FAROESE, FIJIAN, FINNISH, FULAH, GAELIC, GALICIAN, GANDA,
    GUARANI, GUJARATI, HAITIAN, HAUSA, HEBREW, HERERO, HIRI_MOTU, HUNGARIAN,
    ICELANDIC, IDO, IGBO, INDIGENOUS, INTERLINGUA, INUKTITUT, INUPIAQ, IRISH,
    JAVANESE, KALAALLISUT, KANNADA, KANURI, KASHMIRI, KAZAKH, KIKUYU,
    KINYARWANDA, KIRGHIZ, KOMI, KONGO, KUANYAMA, KURDISH, LAO, LATIN,
    LATVIAN, LIMBURGAN, LINGALA, LITHUANIAN, LUBA_KATANGA, LUXEMBOURGISH,
    MACEDONIAN, MALAGASY, MALAYALAM, MALTESE, MANX, MAORI, MARATHI,
    MARSHALLESE, MOLDAVIAN, MONGOLIAN, NAURU, NAVAJO, NDONGA, NEPALI,
    NORTH_NDEBELE, NORTHERN_SAMI, NORWEGIAN, NORWEGIAN_BOKMAL,
    NORWEGIAN_NYNORSK, OCCIDENTAL, OCCITAN, ODIA, OJIBWA, OROMO, OSSETIAN,
    PALI, PERSIAN, PUNJABI, QUECHUA, ROMANIAN, ROMANSH, RUNDI, SAMOAN, SANGO,
    SANSKRIT, SARDINIAN, SHONA, SICHUAN_YI, SINDHI, SINHALA, SLOVAK,
    SLOVENIAN, SOMALI, SOUTH_NDEBELE, SOUTHERN_SOTHO, SUNDANESE, SWAHILI,
    SWATI, SWEDISH, TAGALOG, TAHITIAN, TAJIK, TAMIL, TATAR, TIBETAN,
    TIGRINYA, TONGA, TSONGA, TSWANA, TURKMEN, TWI, UIGHUR, UZBEK, VENDA,
    VOLAPUK, WALLOON, WELSH, WESTERN_FRISIAN, WOLOF, XHOSA, YIDDISH
};

// Helper data for Language mapping (string representation is lowercase)
static const std::vector<std::pair<Language, std::string>> language_map_data = {
    {Language::ARABIC, "arabic"}, {Language::ARMENIAN, "armenian"}, {Language::AZERBAIJANI, "azerbaijani"},
    {Language::BENGALI, "bengali"}, {Language::CHINESE, "chinese"}, {Language::DUTCH, "dutch"},
    {Language::ENGLISH, "english"}, {Language::FRENCH, "french"}, {Language::GEORGIAN, "georgian"},
    {Language::GERMAN, "german"}, {Language::GREEK, "greek"}, {Language::HINDI, "hindi"},
    {Language::INDONESIAN, "indonesian"}, {Language::ITALIAN, "italian"}, {Language::JAPANESE, "japanese"},
    {Language::KOREAN, "korean"}, {Language::MALAYSIAN, "malaysian"}, {Language::PASHTO, "pashto"},
    {Language::POLISH, "polish"}, {Language::PORTUGUESE, "portuguese"}, {Language::RUSSIAN, "russian"},
    {Language::SERBIAN, "serbian"}, {Language::SPANISH, "spanish"}, {Language::TELUGU, "telugu"},
    {Language::THAI, "thai"}, {Language::TURKISH, "turkish"}, {Language::UKRAINIAN, "ukrainian"},
    {Language::URDU, "urdu"}, {Language::VIETNAMESE, "vietnamese"}, {Language::ABKHAZIAN, "abkhazian"},
    {Language::AFAR, "afar"}, {Language::AFRIKAANS, "afrikaans"}, {Language::AKAN, "akan"},
    {Language::YORUBA, "yoruba"}, {Language::ZHUANG, "zhuang"}, {Language::ZULU, "zulu"},
    {Language::ALBANIAN, "albanian"}, {Language::AMHARIC, "amharic"}, {Language::ARAGONESE, "aragonese"},
    {Language::ASSAMESE, "assamese"}, {Language::AVARIC, "avaric"}, {Language::AVESTAN, "avestan"},
    {Language::AYMARA, "aymara"}, {Language::BAMBARA, "bambara"}, {Language::BASHKIR, "bashkir"},
    {Language::BASQUE, "basque"}, {Language::BELARUSIAN, "belarusian"}, {Language::BERBER, "berber"},
    {Language::BISLAMA, "bislama"}, {Language::BOSNIAN, "bosnian"}, {Language::BRETON, "breton"},
    {Language::BULGARIAN, "bulgarian"}, {Language::BURMESE, "burmese"}, {Language::CATALAN, "catalan"},
    {Language::CENTRAL_KHMER, "central_khmer"}, {Language::CHAMORRO, "chamorro"}, {Language::CHECHEN, "chechen"},
    {Language::CHICHEWA, "chichewa"}, {Language::CHURCH_SLAVIC, "church_slavic"}, {Language::CHUVASH, "chuvash"},
    {Language::CORNISH, "cornish"}, {Language::CORSICAN, "corsican"}, {Language::CREE, "cree"},
    {Language::CRIMEAN, "crimean"}, {Language::CROATIAN, "croatian"}, {Language::CZECH, "czech"},
    {Language::DANISH, "danish"}, {Language::DIVEHI, "divehi"}, {Language::DZONGKHA, "dzongkha"},
    {Language::ESPERANTO, "esperanto"}, {Language::ESTONIAN, "estonian"}, {Language::EWE, "ewe"},
    {Language::FAROESE, "faroese"}, {Language::FIJIAN, "fijian"}, {Language::FINNISH, "finnish"},
    {Language::FULAH, "fulah"}, {Language::GAELIC, "gaelic"}, {Language::GALICIAN, "galician"},
    {Language::GANDA, "ganda"}, {Language::GUARANI, "guarani"}, {Language::GUJARATI, "gujarati"},
    {Language::HAITIAN, "haitian"}, {Language::HAUSA, "hausa"}, {Language::HEBREW, "hebrew"},
    {Language::HERERO, "herero"}, {Language::HIRI_MOTU, "hiri_motu"}, {Language::HUNGARIAN, "hungarian"},
    {Language::ICELANDIC, "icelandic"}, {Language::IDO, "ido"}, {Language::IGBO, "igbo"},
    {Language::INDIGENOUS, "indigenous"}, {Language::INTERLINGUA, "interlingua"}, {Language::INUKTITUT, "inuktitut"},
    {Language::INUPIAQ, "inupiaq"}, {Language::IRISH, "irish"}, {Language::JAVANESE, "javanese"},
    {Language::KALAALLISUT, "kalaallisut"}, {Language::KANNADA, "kannada"}, {Language::KANURI, "kanuri"},
    {Language::KASHMIRI, "kashmiri"}, {Language::KAZAKH, "kazakh"}, {Language::KIKUYU, "kikuyu"},
    {Language::KINYARWANDA, "kinyarwanda"}, {Language::KIRGHIZ, "kirghiz"}, {Language::KOMI, "komi"},
    {Language::KONGO, "kongo"}, {Language::KUANYAMA, "kuanyama"}, {Language::KURDISH, "kurdish"},
    {Language::LAO, "lao"}, {Language::LATIN, "latin"}, {Language::LATVIAN, "latvian"},
    {Language::LIMBURGAN, "limburgan"}, {Language::LINGALA, "lingala"}, {Language::LITHUANIAN, "lithuanian"},
    {Language::LUBA_KATANGA, "luba-katanga"}, {Language::LUXEMBOURGISH, "luxembourgish"}, {Language::MACEDONIAN, "macedonian"},
    {Language::MALAGASY, "malagasy"}, {Language::MALAYALAM, "malayalam"}, {Language::MALTESE, "maltese"},
    {Language::MANX, "manx"}, {Language::MAORI, "maori"}, {Language::MARATHI, "marathi"},
    {Language::MARSHALLESE, "marshallese"}, {Language::MOLDAVIAN, "moldavian"}, {Language::MONGOLIAN, "mongolian"},
    {Language::NAURU, "nauru"}, {Language::NAVAJO, "navajo"}, {Language::NDONGA, "ndonga"},
    {Language::NEPALI, "nepali"}, {Language::NORTH_NDEBELE, "north_ndebele"}, {Language::NORTHERN_SAMI, "northern_sami"},
    {Language::NORWEGIAN, "norwegian"}, {Language::NORWEGIAN_BOKMAL, "norwegian_bokmal"}, {Language::NORWEGIAN_NYNORSK, "norwegian_nynorsk"},
    {Language::OCCIDENTAL, "occidental"}, {Language::OCCITAN, "occitan"}, {Language::ODIA, "odia"},
    {Language::OJIBWA, "ojibwa"}, {Language::OROMO, "oromo"}, {Language::OSSETIAN, "ossetian"},
    {Language::PALI, "pali"}, {Language::PERSIAN, "persian"}, {Language::PUNJABI, "punjabi"},
    {Language::QUECHUA, "quechua"}, {Language::ROMANIAN, "romanian"}, {Language::ROMANSH, "romansh"},
    {Language::RUNDI, "rundi"}, {Language::SAMOAN, "samoan"}, {Language::SANGO, "sango"},
    {Language::SANSKRIT, "sanskrit"}, {Language::SARDINIAN, "sardinian"}, {Language::SHONA, "shona"},
    {Language::SICHUAN_YI, "sichuan_yi"}, {Language::SINDHI, "sindhi"}, {Language::SINHALA, "sinhala"},
    {Language::SLOVAK, "slovak"}, {Language::SLOVENIAN, "slovenian"}, {Language::SOMALI, "somali"},
    {Language::SOUTH_NDEBELE, "south_ndebele"}, {Language::SOUTHERN_SOTHO, "southern_sotho"}, {Language::SUNDANESE, "sundanese"},
    {Language::SWAHILI, "swahili"}, {Language::SWATI, "swati"}, {Language::SWEDISH, "swedish"},
    {Language::TAGALOG, "tagalog"}, {Language::TAHITIAN, "tahitian"}, {Language::TAJIK, "tajik"},
    {Language::TAMIL, "tamil"}, {Language::TATAR, "tatar"}, {Language::TIBETAN, "tibetan"},
    {Language::TIGRINYA, "tigrinya"}, {Language::TONGA, "tonga"}, {Language::TSONGA, "tsonga"},
    {Language::TSWANA, "tswana"}, {Language::TURKMEN, "turkmen"}, {Language::TWI, "twi"},
    {Language::UIGHUR, "uighur"}, {Language::UZBEK, "uzbek"}, {Language::VENDA, "venda"},
    {Language::VOLAPUK, "volapuk"}, {Language::WALLOON, "walloon"}, {Language::WELSH, "welsh"},
    {Language::WESTERN_FRISIAN, "western_frisian"}, {Language::WOLOF, "wolof"}, {Language::XHOSA, "xhosa"},
    {Language::YIDDISH, "yiddish"}
};

// Function to get the string representation of Language
ZLIBRARY_API inline std::string toString(Language lang) {
    // For very large enums, a map lookup might be preferred even for enum->string,
    // but linear search is simple.
    for(const auto& pair : language_map_data) {
        if (pair.first == lang) {
            return pair.second;
        }
    }
    throw std::logic_error("Invalid Language enum value"); // Should not happen
}

// Function to get Language from string
ZLIBRARY_API inline Language stringToLanguage(const std::string& s) {
    // Create the map on first call (thread-safe in C++11+)
    static const std::unordered_map<std::string, Language> lookup_map = [] {
        std::unordered_map<std::string, Language> map;
        for (const auto& pair : language_map_data) {
            map[pair.second] = pair.first;
        }
        return map;
    }();

    auto it = lookup_map.find(s);
    if (it == lookup_map.end()) {
        throw std::runtime_error("Invalid string for Language enum: " + s);
    }
    return it->second;
}


}

#endif //ENUMS_HPP
