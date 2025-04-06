// Z-Library/include/API.hpp
#ifndef ZLIBRARYAPI_HPP
#define ZLIBRARYAPI_HPP

#include <string>
#include <functional>
#include <filesystem>
#include "Export.hpp"
#include "Types.hpp"     // Include the new types header
#include <cpr/cpr.h>

namespace fs = std::filesystem;

class ZLIBRARY_API API {
private:
    std::string base_url_;
    cpr::Session session_;
    LogCallback log_callback_;

    void setup_session_defaults();
    void log(LogLevel level, const std::string& module, const std::string& msg);

public:
    // --- Constructors ---
    API();
    explicit API(std::string base_url);
    explicit API(LogCallback logger);
    API(std::string base_url, LogCallback logger);

    // --- Configuration Methods ---
    void setLogCallback(LogCallback logger);
    void setBaseUrl(const std::string& base_url);
    [[nodiscard]] const std::string& getBaseUrl() const;
};

#endif // ZLIBRARYAPI_HPP