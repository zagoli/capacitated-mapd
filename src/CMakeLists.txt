message(STATUS "processing src folder")

# --- Dependencies ---

CPMAddPackage(
        NAME argparse
        GITHUB_REPOSITORY p-ranav/argparse
        VERSION 2.9)

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 9.1.0
)

# --- Source code ----
add_library(distances STATIC
        distances/distances.cpp)
target_include_directories(distances PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(MultiAStar STATIC
        a_star/Node.cpp)
target_include_directories(MultiAStar PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(MultiAStar PRIVATE distances)

add_executable(cmapd main.cpp AmbientMap.cpp AmbientMapInstance.cpp)
target_link_libraries(cmapd PRIVATE argparse::argparse fmt::fmt)