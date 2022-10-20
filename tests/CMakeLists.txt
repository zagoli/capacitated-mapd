message(STATUS "PROCESSING TEST FOLDER")

# ---- Options ----
option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

# Catch2 dependency
CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 3.0.1)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# ---- Create binary ----
add_executable(cmapd_tests
        test_map_class.cpp
        test_distances.cpp
        ${CMAKE_SOURCE_DIR}/src/AmbientMap.cpp)
target_include_directories(cmapd_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cmapd_tests
        Catch2::Catch2WithMain
        distances)

catch_discover_tests(cmapd_tests)

# enable compiler warnings

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(cmapd_tests PUBLIC -Wall -Wpedantic -Wextra -Werror)
endif ()

# ---- code coverage ----

if (ENABLE_TEST_COVERAGE)
    target_compile_options(cmapd_tests PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(cmapd_tests PUBLIC -fprofile-arcs -ftest-coverage)
endif ()
