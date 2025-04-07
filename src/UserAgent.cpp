//
// Created by Advik on 07-04-2025.
//

#include <zlibrary/Utils/UserAgent.hpp>

#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <chrono>
#include <optional>
#include <numeric>
#include <algorithm>
#include <map>
#include <stdexcept> // For potential errors if needed

namespace zlibrary {
namespace utils {

//-----------------------------------------------------------------------------
// Anonymous Namespace for Implementation Details
//-----------------------------------------------------------------------------
namespace {

// --- Internal Enums & Data Structures ---
enum class RenderingEngine { BLINK, GECKO, WEBKIT };

struct DeviceInfo {
    std::string name;
    std::string model_id;
};

struct OsInfo {
    std::string name;
    std::string base_token;
    OsType type;
    Architecture architecture;
    std::vector<BrowserType> common_browsers;
    std::vector<std::string> possible_devices; // Keys into device_database
    std::string arch_token_suffix = "";
    // Removed build_info initialization here; generate it dynamically
    std::string version_comment = ""; // For macOS style version hints
};

struct BrowserInfo {
    std::string name;
    BrowserType type;
    RenderingEngine engine;
    int min_major_version;
    int max_major_version;
    int min_engine_major; // Only relevant for WebKit/Gecko correlation
    int max_engine_major; // Only relevant for WebKit/Gecko correlation
    std::vector<Architecture> supported_architectures;
    std::string specific_token_prefix = "";
};

// --- Global Random Engine (Static Initialization) ---
std::mt19937_64& getRandomEngine() {
    // Seeded once per program execution
    static std::mt19937_64 rng(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    ));
    return rng;
}

// --- Helper Functions ---
template <typename T>
const T& getRandomElement(const std::vector<T>& vec) {
    if (vec.empty()) {
        // Consider throwing a specific exception type if needed
        throw std::runtime_error("Cannot get random element from empty vector.");
    }
    std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
    return vec[dist(getRandomEngine())];
}

int getRandomInt(int min, int max) {
     if (min > max) std::swap(min, max);
     if (min == max) return min;
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getRandomEngine());
}

std::string generateComplexVersion(int major, int parts = 4) {
    std::ostringstream version_stream;
    version_stream << major;
    if (parts >= 2) version_stream << "." << 0;
    if (parts >= 3) version_stream << "." << getRandomInt(5500, 6500);
    if (parts >= 4) version_stream << "." << getRandomInt(50, 250);
    return version_stream.str();
}

std::string generateWebKitVersion(int safari_major) {
     int base_webkit = 605;
     if (safari_major >= 16) base_webkit = 610 + (safari_major - 16) * 2;
     if (safari_major >= 17) base_webkit = 614 + (safari_major - 17);

     int webkit_major = getRandomInt(base_webkit, base_webkit + 5);
     std::ostringstream version_stream;
     version_stream << webkit_major << ".1." << getRandomInt(5, 30);
     return version_stream.str();
}

std::string generateGeckoTrail() {
     int year = getRandomInt(2022, 2023);
     int month = getRandomInt(1, 12);
     int day = getRandomInt(1, 28);
     std::ostringstream date_stream;
     date_stream << year << (month < 10 ? "0" : "") << month << (day < 10 ? "0" : "") << day;
     return date_stream.str();
}

std::string generateMobileBuildToken(OsType os_type) {
    if (os_type == OsType::IOS) {
        return std::to_string(getRandomInt(18, 21)) +
               (char)('A' + getRandomInt(0, 10)) +
               std::to_string(getRandomInt(100, 800));
    } else if (os_type == OsType::ANDROID) {
        std::string p1 = (getRandomInt(0, 1) == 0 ? "TP1A." : "SP1A.");
        std::string p2 = std::to_string(getRandomInt(21, 23)) +
                         (getRandomInt(0, 1) == 0 ? "0" : "1") +
                         std::to_string(getRandomInt(1, 9)) +
                         std::to_string(getRandomInt(10, 30));
        std::string p3 = ".0" + std::to_string(getRandomInt(10, 50));
        return p1 + p2 + p3;
    }
    return "";
}


// --- Core Data Definitions ---

const std::map<std::string, DeviceInfo> device_database = {
    {"Pixel 7", {"Pixel 7", "Pixel 7"}},
    {"Pixel 6 Pro", {"Pixel 6 Pro", "Pixel 6 Pro"}},
    {"Galaxy S23", {"Samsung Galaxy S23", "SM-S918U"}},
    {"Galaxy S22", {"Samsung Galaxy S22", "SM-S901U"}},
    {"Galaxy Tab S8", {"Samsung Galaxy Tab S8", "SM-X700"}},
    {"Generic Android", {"Generic Android", "Generic Mobile"}},
    {"iPhone 14 Pro", {"iPhone", "iPhone"}},
    {"iPhone SE", {"iPhone", "iPhone"}},
    {"iPad Pro", {"iPad", "iPad"}},
    {"iPad Air", {"iPad", "iPad"}}
};

// Note: Removed dynamic build token generation from static initialization
const std::vector<OsInfo> os_database = {
    // Windows
    {"Win11 x64", "Windows NT 10.0", OsType::WINDOWS, Architecture::X86_64, {BrowserType::CHROME, BrowserType::EDGE, BrowserType::FIREFOX, BrowserType::OPERA}, {}, "; Win64; x64"},
    {"Win11 ARM", "Windows NT 10.0", OsType::WINDOWS, Architecture::ARM64, {BrowserType::EDGE, BrowserType::CHROME, BrowserType::FIREFOX}, {}, "; ARM64"},
    {"Win10 x64", "Windows NT 10.0", OsType::WINDOWS, Architecture::X86_64, {BrowserType::CHROME, BrowserType::EDGE, BrowserType::FIREFOX, BrowserType::OPERA}, {}, "; Win64; x64"},
    {"Win10 32on64", "Windows NT 10.0", OsType::WINDOWS, Architecture::WOW64, {BrowserType::CHROME, BrowserType::EDGE, BrowserType::FIREFOX, BrowserType::OPERA}, {}, "; WOW64"},
    // macOS (Using version_comment for OS version hint)
    {"macOS Sonoma Intel", "Macintosh; Intel Mac OS X 10_15_7", OsType::MACOS, Architecture::X86_64, {BrowserType::SAFARI, BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::EDGE, BrowserType::OPERA}, {}, "", "; macOS 14.0"},
    {"macOS Ventura Intel", "Macintosh; Intel Mac OS X 10_15_7", OsType::MACOS, Architecture::X86_64, {BrowserType::SAFARI, BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::EDGE, BrowserType::OPERA}, {}, "", "; macOS 13.5"},
    {"macOS Sonoma ARM", "Macintosh; Arm Mac OS X 10_15_7", OsType::MACOS, Architecture::ARM64, {BrowserType::SAFARI, BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::EDGE, BrowserType::OPERA}, {}, "", "; macOS 14.0"},
    {"macOS Ventura ARM", "Macintosh; Arm Mac OS X 10_15_7", OsType::MACOS, Architecture::ARM64, {BrowserType::SAFARI, BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::EDGE, BrowserType::OPERA}, {}, "", "; macOS 13.5"},
     // Linux
    {"Ubuntu x64", "X11; Ubuntu; Linux x86_64", OsType::LINUX, Architecture::X86_64, {BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::OPERA}, {}},
    {"Linux Generic x64", "X11; Linux x86_64", OsType::LINUX, Architecture::X86_64, {BrowserType::CHROME, BrowserType::FIREFOX, BrowserType::OPERA}, {}},
     // iOS
    {"iOS 17", "iPhone; CPU iPhone OS 17_1 like Mac OS X", OsType::IOS, Architecture::ARM64, {BrowserType::SAFARI}, {"iPhone 14 Pro", "iPhone SE"}},
    {"iOS 16 iPad", "iPad; CPU OS 16_6 like Mac OS X", OsType::IOS, Architecture::ARM64, {BrowserType::SAFARI}, {"iPad Pro", "iPad Air"}},
     // Android
    {"Android 14", "Linux; Android 14", OsType::ANDROID, Architecture::ARM64, {BrowserType::CHROME, BrowserType::SAMSUNG_INTERNET, BrowserType::FIREFOX, BrowserType::OPERA}, {"Pixel 7", "Galaxy S23"}},
    {"Android 13", "Linux; Android 13", OsType::ANDROID, Architecture::ARM64, {BrowserType::CHROME, BrowserType::SAMSUNG_INTERNET, BrowserType::FIREFOX, BrowserType::OPERA}, {"Pixel 6 Pro", "Galaxy S22", "Galaxy Tab S8"}},
    {"Android 12", "Linux; Android 12", OsType::ANDROID, Architecture::ARM64, {BrowserType::CHROME, BrowserType::SAMSUNG_INTERNET, BrowserType::FIREFOX}, {"Generic Android"}},
};

const std::vector<BrowserInfo> browser_database = {
    {"Chrome", BrowserType::CHROME, RenderingEngine::BLINK, 115, 125, 537, 537, {Architecture::X86_64, Architecture::ARM64, Architecture::WOW64}, ""},
    {"Firefox", BrowserType::FIREFOX, RenderingEngine::GECKO, 115, 125, 115, 125, {Architecture::X86_64, Architecture::ARM64, Architecture::WOW64}, ""},
    {"Safari", BrowserType::SAFARI, RenderingEngine::WEBKIT, 16, 17, 610, 616, {Architecture::X86_64, Architecture::ARM64}, ""},
    {"Edge", BrowserType::EDGE, RenderingEngine::BLINK, 115, 125, 537, 537, {Architecture::X86_64, Architecture::ARM64, Architecture::WOW64}, "Edg/"},
    {"Opera", BrowserType::OPERA, RenderingEngine::BLINK, 100, 105, 537, 537, {Architecture::X86_64, Architecture::ARM64, Architecture::WOW64}, "OPR/"},
    {"Samsung Internet", BrowserType::SAMSUNG_INTERNET, RenderingEngine::BLINK, 20, 23, 537, 537, {Architecture::ARM64}, "SamsungBrowser/"},
};


} // End anonymous namespace


//-----------------------------------------------------------------------------
// Public Function Implementation
//-----------------------------------------------------------------------------

std::string generateUltraUserAgent(
    OsType desired_os,
    BrowserType desired_browser,
    Architecture desired_arch,
    bool simulate_webview)
{
    // Use anonymous namespace functions/data directly
    auto& rng = getRandomEngine(); // Get the engine instance

    // --- Steps 1-7 from previous implementation ---
    // (The entire logic of selecting OS, Browser, Device, generating versions,
    // and assembling the string goes here, referencing the data and helper
    // functions defined in the anonymous namespace above).
    // ... (Copy the full logic from the previous `generateUltraUserAgent` here) ...

    // --- Placeholder for the logic (copy from previous answer) ---
    // 1. Filter OS List
    std::vector<const OsInfo*> available_os;
    for (const auto& os : os_database) {
        bool os_match = (desired_os == OsType::ANY || os.type == desired_os);
        bool arch_match = (desired_arch == Architecture::ANY || os.architecture == desired_arch);
         if (desired_arch == Architecture::WOW64 && os.type == OsType::WINDOWS && os.architecture == Architecture::X86_64) {
             bool found_wow64_variant = false;
             for (const auto& os_check : os_database) {
                 if (os_check.type == OsType::WINDOWS && os_check.architecture == Architecture::WOW64 && os_check.base_token == os.base_token) {
                     found_wow64_variant = true; break;
                 }
             }
             // Only allow direct x64 OS match for WOW64 if NO specific WOW64 variant exists for that base token
             if(found_wow64_variant) {
                arch_match = (os.architecture == Architecture::WOW64); // Must match the specific WOW64 entry if one exists
             } else {
                 arch_match = true; // Allow x64 base if no specific WOW64 entry for this Win version
             }
         } else if (desired_arch == Architecture::X86_64 && os.architecture == Architecture::WOW64) {
              arch_match = false;
         }
        if (os_match && arch_match) {
            available_os.push_back(&os);
        }
    }
     if (available_os.empty()) { // Fallback 1: Relax architecture
        for (const auto& os : os_database) {
            if (desired_os == OsType::ANY || os.type == desired_os) available_os.push_back(&os);
        }
     }
    if (available_os.empty()) { // Fallback 2: Relax OS
             for (const auto& os : os_database) available_os.push_back(&os);
             if(available_os.empty()) return "Error: No OS in database."; // Should not happen
    }

    // 2. Select OS
    const OsInfo* selected_os = getRandomElement(available_os);
    Architecture final_architecture = selected_os->architecture; // Arch is determined by the chosen OS entry

    // 3. Select Device (if mobile) & Generate dynamic parts
    const DeviceInfo* selected_device = nullptr;
    std::string device_token_part = "";
    std::string android_build_token = ""; // Generate dynamically if needed

     if (!selected_os->possible_devices.empty()) {
        const std::string& device_key = getRandomElement(selected_os->possible_devices);
        if (device_database.count(device_key)) {
            selected_device = &device_database.at(device_key);
            if (selected_os->type == OsType::ANDROID && selected_device->model_id != "Generic Mobile") {
                 if(selected_os->base_token.find(selected_device->model_id) == std::string::npos) {
                     device_token_part = "; " + selected_device->model_id;
                 }
            }
        }
    }
    // Generate Android build token *now*
    if (selected_os->type == OsType::ANDROID) {
        android_build_token = "; Build/" + generateMobileBuildToken(OsType::ANDROID);
    }


    // --- WebView Simulation ---
     if (simulate_webview && selected_os->type == OsType::ANDROID) {
         // Find Chrome info for versioning the WebView UA
         const BrowserInfo* chrome_info = nullptr;
         for(const auto& browser : browser_database) {
             if (browser.type == BrowserType::CHROME) { chrome_info = &browser; break; }
         }
         if (!chrome_info) return "Error: Chrome browser info not found for WebView."; // Should not happen

        int chrome_major = getRandomInt(chrome_info->min_major_version, chrome_info->max_major_version);
        std::string webkit_ver = "537.36";
        std::ostringstream wv_ua;
        wv_ua << "Mozilla/5.0 (" << selected_os->base_token << device_token_part << android_build_token // Add build token here
              << ") AppleWebKit/" << webkit_ver << " (KHTML, like Gecko) Version/4.0 Chrome/"
              << chrome_major << ".0." << getRandomInt(5500, 6500) << "." << getRandomInt(50, 250)
              << " Mobile Safari/" << webkit_ver;
        return wv_ua.str();
    }


    // 4. Filter Browser List
     std::vector<const BrowserInfo*> available_browsers;
    for (const auto& browser : browser_database) {
        bool type_match = (desired_browser == BrowserType::ANY || browser.type == desired_browser);
        bool os_compatible = false;
        for(const auto& compatible_type : selected_os->common_browsers) {
            if (browser.type == compatible_type) { os_compatible = true; break; }
        }
        bool arch_compatible = false;
        for(const auto& supported_arch : browser.supported_architectures) {
            if (supported_arch == final_architecture) { arch_compatible = true; break; }
            // Allow x86 browser on WOW64 OS entry
            if (final_architecture == Architecture::WOW64 && supported_arch == Architecture::X86_64) { arch_compatible = true; break; }
        }
         // Rosetta 2 emulation for macOS ARM
        if (final_architecture == Architecture::ARM64 && selected_os->type == OsType::MACOS) {
             for(const auto& supported_arch : browser.supported_architectures) {
                 if (supported_arch == Architecture::X86_64) { arch_compatible = true; break; }
             }
        }

        if (selected_os->type == OsType::IOS) {
             if (browser.type == BrowserType::SAFARI) { available_browsers.push_back(&browser); break; }
        } else if (type_match && os_compatible && arch_compatible) {
            available_browsers.push_back(&browser);
        }
    }
     if (available_browsers.empty()) { // Fallback: Relax browser constraint
        for (const auto& browser : browser_database) {
             bool os_compatible = false;
             for(const auto& compatible_type : selected_os->common_browsers) { if (browser.type == compatible_type) { os_compatible = true; break; } }
             bool arch_compatible = false;
             for(const auto& supported_arch : browser.supported_architectures) {
                 if (supported_arch == final_architecture) { arch_compatible = true; break; }
                 if (final_architecture == Architecture::WOW64 && supported_arch == Architecture::X86_64) { arch_compatible = true; break;}
                 if (final_architecture == Architecture::ARM64 && selected_os->type == OsType::MACOS && supported_arch == Architecture::X86_64) { arch_compatible = true; break;}
             }
             if (selected_os->type == OsType::IOS) {
                 if (browser.type == BrowserType::SAFARI) { available_browsers.push_back(&browser); break;}
             } else if (os_compatible && arch_compatible) {
                 available_browsers.push_back(&browser);
             }
        }
         if (available_browsers.empty()) return "Error: No suitable browser found for OS/Arch.";
    }


    // 5. Select Browser
    const BrowserInfo* selected_browser = getRandomElement(available_browsers);

    // 6. Generate Versions
    int major_version = getRandomInt(selected_browser->min_major_version, selected_browser->max_major_version);
    std::string browser_version_str = generateComplexVersion(major_version);
    std::string engine_version_str = "";
    std::string gecko_trail = "";

     if (selected_browser->engine == RenderingEngine::WEBKIT) {
        int basis_major = major_version; // Use own major version if Safari
        if (selected_browser->type != BrowserType::SAFARI) { // Should only happen on iOS if not Safari-like structure used
           const BrowserInfo* safari_info = nullptr;
           for(const auto& binfo : browser_database) if(binfo.type == BrowserType::SAFARI) safari_info = &binfo;
           if(safari_info) basis_major = getRandomInt(safari_info->min_major_version, safari_info->max_major_version);
        }
         engine_version_str = generateWebKitVersion(basis_major);
    } else if (selected_browser->engine == RenderingEngine::GECKO) {
        engine_version_str = std::to_string(major_version) + ".0";
        gecko_trail = generateGeckoTrail();
    } else { // BLINK
        engine_version_str = "537.36";
    }

    // 7. Assemble the UA String
    std::ostringstream ua_stream;
    std::string platform_token = selected_os->base_token;
    if (!device_token_part.empty()) platform_token += device_token_part;
    if (!selected_os->arch_token_suffix.empty()) platform_token += selected_os->arch_token_suffix;
    if (selected_os->type == OsType::ANDROID) platform_token += android_build_token; // Add dynamic build token
    if (!selected_os->version_comment.empty() && selected_os->type == OsType::MACOS) platform_token += selected_os->version_comment;

    ua_stream << "Mozilla/5.0 (" << platform_token;
    if (selected_browser->engine == RenderingEngine::GECKO) ua_stream << "; rv:" << engine_version_str;
    ua_stream << ")";

     if (selected_browser->engine == RenderingEngine::WEBKIT || selected_browser->engine == RenderingEngine::BLINK) {
        ua_stream << " AppleWebKit/" << engine_version_str << " (KHTML, like Gecko)";
    } else if (selected_browser->engine == RenderingEngine::GECKO) {
        ua_stream << " Gecko/" << gecko_trail;
    }

    bool add_mobile_token = (selected_os->type == OsType::ANDROID || selected_os->type == OsType::IOS);

    if (selected_browser->engine == RenderingEngine::WEBKIT) { // Covers Safari and iOS-forced WebKit
         std::string safari_style_version = std::to_string(major_version) + "." + std::to_string(getRandomInt(0, 5));
         ua_stream << " Version/" << safari_style_version;
         if (add_mobile_token && selected_os->type == OsType::IOS) {
             ua_stream << " Mobile/" << generateMobileBuildToken(OsType::IOS);
         }
         ua_stream << " Safari/" << engine_version_str;
    }
    else if (selected_browser->engine == RenderingEngine::GECKO) { // Firefox
        ua_stream << " Firefox/" << browser_version_str;
    }
    else if (selected_browser->engine == RenderingEngine::BLINK) { // Chrome, Edge, Opera, Samsung
         ua_stream << " " << selected_browser->name << "/" << browser_version_str;
         if (!selected_browser->specific_token_prefix.empty()) {
             ua_stream << " " << selected_browser->specific_token_prefix << browser_version_str;
         }
         if (add_mobile_token && selected_os->type == OsType::ANDROID) {
             ua_stream << " Mobile";
         }
         ua_stream << " Safari/" << engine_version_str; // Blink compatibility token
    }

    return ua_stream.str();

}

} // namespace utils
} // namespace zlibrary