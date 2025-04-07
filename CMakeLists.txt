cmake_minimum_required(VERSION 3.14) # FetchContent_MakeAvailable needs 3.14+
project("Z Library Wrapper")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# This global option affects dependencies if they check for it.
# Let's keep it OFF, meaning dependencies default to STATIC unless they override.
option(BUILD_SHARED_LIBS "Build dependencies as shared libraries" OFF)

# --- MSVC Runtime Library Handling ---
# Ensure all components use the DLL runtime when building the main lib as SHARED.
# This setting needs to be set BEFORE dependencies are configured by FetchContent_MakeAvailable.
if(MSVC)
    # Use the DLL runtime (/MD or /MDd)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC Runtime Library")
    # Propagate the CMAKE_BUILD_TYPE to dependencies if using a multi-config generator (like Visual Studio)
    # This helps ensure dependencies get the correct Debug/Release flags matching the main project.
    set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
endif()
# --- End MSVC Runtime ---

include(FetchContent)

# Declare HtmlParser dependency
FetchContent_Declare(
        HtmlParser
        GIT_REPOSITORY https://github.com/JustCabbage/HtmlParser.git
        GIT_TAG master # Consider using a specific commit/tag for stability
)
# Configure HtmlParser options BEFORE MakeAvailable
option(HTMLPARSER_BUILD_TESTS "Build tests" OFF)
option(HTMLPARSER_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(HTMLPARSER_BUILD_EXAMPLES "Build examples" OFF)
# Explicitly tell HtmlParser to build static if it checks BUILD_SHARED_LIBS
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build HtmlParser static" FORCE)

# Declare utf8cpp dependency (header-only)
FetchContent_Declare(
        utf8cpp
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG v4.0.6
)

# Declare nlohmann/json dependency (header-only)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

# Declare cpr dependency
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.11.2
)
# Configure cpr options BEFORE MakeAvailable
option(CPR_BUILD_TESTS "Build CPR tests" OFF)
set(CPR_BUILD_TESTS OFF CACHE BOOL "Build CPR tests" FORCE) # Force OFF
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "Use system curl")
# Explicitly tell CPR to build static (it likely defaults to this anyway if BUILD_SHARED_LIBS is OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build CPR static" FORCE)


# Make dependencies available *after* configuring options and MSVC runtime
FetchContent_MakeAvailable(HtmlParser utf8cpp nlohmann_json cpr)
# Restore global BUILD_SHARED_LIBS state if needed elsewhere, though usually not necessary
# unset(BUILD_SHARED_LIBS CACHE)


file(GLOB_RECURSE SOURCES src/*.cpp)
# GLOB is convenient but re-run CMake if you add/remove files.
# Headers don't need to be listed for add_library unless used for custom commands.
# Listing them doesn't hurt, but isn't standard practice.
# file(GLOB_RECURSE HEADERS include/*.hpp)

# Define the main library as SHARED
add_library(zlibrary SHARED ${SOURCES}) # Removed HEADERS from here

# Add export macro definition *only* when building the DLL itself
# ZLIBRARY_EXPORTS should enable __declspec(dllexport)
# Clients linking against the DLL should *not* define ZLIBRARY_EXPORTS
# ZLIBRARY_API macro in headers should use ZLIBRARY_EXPORTS to switch
# between __declspec(dllexport) and __declspec(dllimport).
if(WIN32)
    target_compile_definitions(zlibrary PRIVATE ZLIBRARY_EXPORTS)
endif()

# Public include directory for consumers
target_include_directories(zlibrary PUBLIC include)

# Link dependencies
# Use the target names provided by the dependencies' CMake scripts
# utf8cpp and nlohmann_json are header-only, MakeAvailable sets up include dirs.
# Linking them explicitly might be needed if they create INTERFACE targets. Check their CMake files.
# Assuming HtmlParser creates target 'HtmlParser' and utf8cpp creates 'utf8cpp' (INTERFACE).
# cpr creates 'cpr::cpr'. nlohmann_json creates 'nlohmann_json::nlohmann_json'.
target_link_libraries(zlibrary
        PUBLIC
        HtmlParser # Check if HtmlParser::HtmlParser exists
        # utf8cpp::utf8cpp # Often INTERFACE targets exist, check utf8cpp CMake
        cpr::cpr nlohmann_json
        )

# If utf8cpp is truly header-only with no target, just ensure include dirs are set (MakeAvailable does this)
# target_link_libraries(zlibrary PRIVATE utf8cpp) # Link if it provides an INTERFACE library target

# --- Test Executable ---
add_executable(zlibrary_test test/main.cpp)
target_link_libraries(zlibrary_test PRIVATE zlibrary)

# --- Platform specific settings for MSVC ---
if(MSVC)
    # Apply common settings to both targets
    foreach(TARGET zlibrary zlibrary_test)
        target_compile_options(${TARGET} PRIVATE /W4 /EHsc /utf-8)
    endforeach()

    # Optional: Suppress C4251 warning if not using PIMPL yet
    # target_compile_options(zlibrary PRIVATE /wd4251)
endif()