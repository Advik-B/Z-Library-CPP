cmake_minimum_required(VERSION 4.0)
project("Z Library Wrapper")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


include(FetchContent)
FetchContent_Declare(
        HtmlParser
        GIT_REPOSITORY https://github.com/JustCabbage/HtmlParser.git
        GIT_TAG master
)

option(HTMLPARSER_BUILD_TESTS "Build tests" OFF)
option(HTMLPARSER_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(HTMLPARSER_BUILD_EXAMPLES "Build examples" OFF)

FetchContent_Declare(
        utf8cpp # Name you'll use to refer to it
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG v4.0.6 # Use a specific tag for stability!
)

# --- Add nlohmann/json dependency ---
FetchContent_Declare(
        nlohmann_json             # Name
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3           # Use a specific stable tag
)
FetchContent_MakeAvailable(nlohmann_json)
# --- End Add nlohmann/json ---

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.11.2
)
option(CPR_BUILD_TESTS "Build CPR tests" OFF)
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "Use system curl")

FetchContent_MakeAvailable(htmlparser utf8cpp cpr nlohmann_json)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_library(zlibrary SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(zlibrary PRIVATE ZLIBRARY_EXPORTS)
target_include_directories(zlibrary PUBLIC include)
target_link_libraries(zlibrary PRIVATE HtmlParser utf8cpp cpr nlohmann_json::nlohmann_json)

add_executable(zlibrary_test test/main.cpp)
target_link_libraries(zlibrary_test PRIVATE zlibrary)

# --- Platform specific settings for MSVC (Optional but good practice) ---
if(MSVC)
    target_compile_options(zlibrary PRIVATE /W4 /EHsc)
    # Add flag for UTF-8 source/execution character sets (requires VS 2015 Update 2+)
    target_compile_options(zlibrary PRIVATE /utf-8)
endif()