cmake_minimum_required(VERSION 3.22)

# --- Project ---
project(
        CMAPD
        VERSION 1.0
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Include guards ---
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

# --- Dependencies ---
include(CPM)
# Download dependencies here
set(CPM_SOURCE_CACHE 
        "${CMAKE_SOURCE_DIR}/third_party/cpm_sources" 
        CACHE PATH "Directory to download CPM dependencies" 
        FORCE)

# --- Add source files ---
add_subdirectory(src)

# --- Add tests ---
add_subdirectory(tests)