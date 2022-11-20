cmake_minimum_required(VERSION 3.22)

# --- Project ---
project(
        CMAPD
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find custom scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third_party/or_tools")

# testing
enable_testing()

# --- Include guards ---
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

# --- Dependencies manager ---
# Download dependencies here
set(CPM_SOURCE_CACHE
        "${CMAKE_SOURCE_DIR}/third_party/cpm_sources"
        CACHE PATH "Directory to download CPM dependencies"
        FORCE)
# Include dependency manager
include(CPM)
# Include additional tools
include(tools)
# Catch2 dependency
CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 3.0.1
        EXCLUDE_FROM_ALL TRUE)
# argparse dependency
CPMAddPackage(
        NAME argparse
        GITHUB_REPOSITORY p-ranav/argparse
        VERSION 2.9
        EXCLUDE_FROM_ALL TRUE)
# fmt dependency
CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 9.1.0
        EXCLUDE_FROM_ALL TRUE)
# ortools - BINARIES FOR UBUNTU 22.04 INCLUDED
find_package(ortools REQUIRED CONFIG)

# --- Add source files ---
add_subdirectory(src)

# --- Add tests ---
add_subdirectory(tests)

# --- Add documentation ---
add_subdirectory(doc)

# --- Install custom target ---
# get sure the copied target finds shared libraries
set_target_properties(cmapd PROPERTIES INSTALL_RPATH ${PROJECT_SOURCE_DIR}/third_party/or_tools/lib)

set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS cmapd
        RUNTIME DESTINATION ${INSTALL_DIR})

# --- code coverage ---
OPTION(COVERAGE "Enables code coverage report files creation")
if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang") AND ${COVERAGE})
    target_compile_options(cmapd_tests PUBLIC -g --coverage -O0)
    target_link_options(cmapd_tests PUBLIC --coverage)

    target_compile_options(distances PUBLIC -g --coverage -O0)
    target_link_options(distances PUBLIC --coverage)

    target_compile_options(multi_a_star PUBLIC -g --coverage -O0)
    target_link_options(multi_a_star PUBLIC --coverage)

    target_compile_options(path_finders PUBLIC -g --coverage -O0)
    target_link_options(path_finders PUBLIC --coverage)

    target_compile_options(task_assigner PUBLIC -g --coverage -O0)
    target_link_options(task_assigner PUBLIC --coverage)

    target_compile_options(cmapd PUBLIC -g --coverage -O0)
    target_link_options(cmapd PUBLIC --coverage)
endif ()
