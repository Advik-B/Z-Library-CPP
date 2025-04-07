#ifndef ZLIBRARY_UTILS_USER_AGENT_HPP
#define ZLIBRARY_UTILS_USER_AGENT_HPP

#include <string>
#include <vector> // Needed for return types if we ever changed the API, good practice
#include <optional> // If needed for future API changes, good practice
#include <zlibrary/Export.hpp>

namespace zlibrary {
    namespace utils {

        // --- Public Enums for Configuration ---
        enum class OsType { ANY, WINDOWS, MACOS, LINUX, IOS, ANDROID };
        enum class BrowserType { ANY, CHROME, FIREFOX, SAFARI, EDGE, OPERA, SAMSUNG_INTERNET };
        enum class Architecture { ANY, X86_64, ARM64, WOW64 };

        /**
         * @brief Generates a plausible, highly randomized, and structurally valid browser user agent string.
         *
         * This function attempts to create realistic user agents by combining various OS,
         * browser, architecture, and device details based on common patterns.
         *
         * @param desired_os Optional: Filter generation to only include this OS type.
         * @param desired_browser Optional: Filter generation to only include this browser type.
         *                   Note: For OsType::IOS, the generated UA will always resemble Safari
         *                   due to iOS engine restrictions, regardless of this parameter.
         * @param desired_arch Optional: Filter generation to only include this architecture.
         *                     Takes into account compatibility (e.g., WOW64 on Windows x64,
         *                     x86_64 emulation on macOS ARM).
         * @param simulate_webview Optional: If true and the selected OS is Android, generates
         *                        a UA string resembling an Android System WebView UA.
         * @return A generated user agent string, or an error message if generation fails.
         */
        ZLIBRARY_API std::string generateUltraUserAgent(
            OsType desired_os = OsType::ANY,
            BrowserType desired_browser = BrowserType::ANY,
            Architecture desired_arch = Architecture::ANY,
            bool simulate_webview = false);

    } // namespace utils
} // namespace zlibrary

#endif // ZLIBRARY_UTILS_USER_AGENT_HPP