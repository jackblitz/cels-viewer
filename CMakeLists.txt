cmake_minimum_required(VERSION 3.16)
project(cels_viewer C)

# Use C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic)

# Output directories
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_lower)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/build/${build_type_lower})
set(OUT_DIR ${CMAKE_SOURCE_DIR}/out)

# Ensure out/ directory exists
file(MAKE_DIRECTORY ${OUT_DIR})

# Set output directories for all builds to out/
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

# Dependencies
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# cJSON
add_subdirectory(vendor/cJSON)

# curl
set(BUILD_TESTING OFF CACHE INTERNAL "")
set(BUILD_CURL_SAMPLES OFF CACHE INTERNAL "")
set(BUILD_CURL_DOCS OFF CACHE INTERNAL "")
add_subdirectory(vendor/curl)

# ncurses
find_package(Curses REQUIRED)

# Set target output directories to out/
set_target_properties(cjson PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
)

set_target_properties(libcurl_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
)

# Main executable - output to build/
add_executable(cels_viewer src/main.c src/app.c)
target_include_directories(cels_viewer PRIVATE 
    include 
    vendor/cJSON
    ${CURSES_INCLUDE_DIRS}
)
target_link_libraries(cels_viewer PRIVATE 
    cjson 
    libcurl 
    ${CURSES_LIBRARIES}
)
set_target_properties(cels_viewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}
)

# Test executable - output to build/
add_executable(test_app tests/test_app.c src/app.c)
target_include_directories(test_app PRIVATE
    include
    vendor/cJSON
)
target_link_libraries(test_app PRIVATE
    cjson
    ${CURSES_LIBRARIES}
)
set_target_properties(test_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}
)
