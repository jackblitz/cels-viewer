cmake_minimum_required(VERSION 3.16)
project(cels_viewer C)

# Use C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic)

# Output directories
# Set runtime output to build/debug or build/release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

# Dependencies
# cJSON: Simple target-based integration
add_subdirectory(vendor/cJSON)

# curl: Complex build, we only need the library
# We disable tests and docs to speed up and avoid issues
set(BUILD_TESTING OFF CACHE INTERNAL "")
set(BUILD_CURL_SAMPLES OFF CACHE INTERNAL "")
set(BUILD_CURL_DOCS OFF CACHE INTERNAL "")
add_subdirectory(vendor/curl)

# ncurses: No CMakeLists.txt provided by upstream.
# For now, we use the system ncurses as a fallback to make the build system work,
# but we will look for a way to vendor it properly.
find_package(Curses REQUIRED)

# Executable
add_executable(cels_viewer src/main.c)

# Include directories
target_include_directories(cels_viewer PRIVATE 
    include 
    vendor/cJSON
    ${CURSES_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(cels_viewer PRIVATE 
    cjson 
    libcurl 
    ${CURSES_LIBRARIES}
)
