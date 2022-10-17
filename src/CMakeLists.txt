message(STATUS "processing src folder")

# ---- Dependencies ----

CPMAddPackage(
        NAME argparse
        GITHUB_REPOSITORY p-ranav/argparse
        VERSION 2.9)

# ---- Source code -----
add_executable(cmapd main.cpp AmbientMap.cpp AmbientMap.h Point.h)
target_link_libraries(cmapd argparse)