cmake_minimum_required(VERSION 3.16)
project(cels_viewer C)

# Use C99 standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Application binaries go to build/<config>. Everything else (CMake files,
# object files, vendored libraries) stays in the CMake binary dir, which the
# presets place under out/<config> — configure with `cmake --preset debug`.
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_lower)
set(APP_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/${build_type_lower})

# --- Vendored dependencies: static libraries only ---
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# cJSON: skip its test executables
set(ENABLE_CJSON_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/cJSON EXCLUDE_FROM_ALL)

# curl: libcurl only — no curl CLI, tests, or install targets
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/curl EXCLUDE_FROM_ALL)

# ncurses
find_package(Curses REQUIRED)

# Warnings for our own targets only (declared after the vendored deps)
add_compile_options(-Wall -Wextra -Wpedantic)

# Main executable
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

# cels_tests: CLI test harness (vendored greatest framework). Each feature
# under tests/features/ contributes a suite; run one with
# `cels_tests -s <suite>`. Links libcurl so linking checks resolve.
add_executable(cels_tests
    tests/cli/main.c
    tests/features/foundation_suite.c
    src/app.c
)
target_include_directories(cels_tests PRIVATE
    include
    vendor/cJSON
    vendor/greatest
    ${CURSES_INCLUDE_DIRS}
)
target_link_libraries(cels_tests PRIVATE
    cjson
    libcurl
    ${CURSES_LIBRARIES}
)

set_target_properties(cels_viewer cels_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${APP_OUTPUT_DIR}
)
