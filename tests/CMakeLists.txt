message(STATUS "PROCESSING TEST FOLDER")

# --- Copy test data folder into build folder ---
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# --- Options ---
option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# --- Create binary ---
add_executable(cmapd_tests
        test_map_class.cpp
        test_map_instance_class.cpp
        test_distances.cpp
        ${CMAKE_SOURCE_DIR}/src/AmbientMap.cpp
        ${CMAKE_SOURCE_DIR}/src/AmbientMapInstance.cpp)
target_include_directories(cmapd_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cmapd_tests
        PRIVATE
        distances
        Catch2::Catch2WithMain
        fmt::fmt)

catch_discover_tests(cmapd_tests)

# enable compiler warnings

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(cmapd_tests PUBLIC -Wall -Wpedantic -Wextra -Werror)
endif ()

# --- code coverage ---

if (ENABLE_TEST_COVERAGE)
    target_compile_options(cmapd_tests PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(cmapd_tests PUBLIC -fprofile-arcs -ftest-coverage)
endif ()
