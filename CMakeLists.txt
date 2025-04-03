cmake_minimum_required(VERSION 4.0)
project("Z Library Wrapper")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
FetchContent_MakeAvailable(htmlparser utf8cpp)

file(GLOB_RECURSE SOURCES Z-Library/src/*.cpp)
file(GLOB_RECURSE HEADERS Z-Library/include/*.hpp)

add_library(zlibrary SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(zlibrary PRIVATE ZLIBRARY_EXPORTS)
target_include_directories(zlibrary PUBLIC Z-Library/include)
target_link_libraries(zlibrary PRIVATE HtmlParser utf8cpp)

#add_library(zlibrary_static STATIC ${SOURCES} ${HEADERS})
#target_compile_definitions(zlibrary_static PRIVATE ZLIBRARY_STATIC)
#target_include_directories(zlibrary_static PUBLIC Z-Library/include)
#target_link_libraries(zlibrary_static PRIVATE HtmlParser utf8cpp)


add_executable(zlibrary_test test/main.cpp)
target_link_libraries(zlibrary_test PRIVATE zlibrary)

# --- Platform specific settings for MSVC (Optional but good practice) ---
if(MSVC)
    target_compile_options(zlibrary PRIVATE /W4 /EHsc)
    # Add flag for UTF-8 source/execution character sets (requires VS 2015 Update 2+)
    target_compile_options(zlibrary PRIVATE /utf-8)
endif()