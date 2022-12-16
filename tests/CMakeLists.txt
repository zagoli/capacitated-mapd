message(DEBUG "processing test folder")

# --- Copy test data folder into build folder ---
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# --- Options ---
option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# --- Create test binary ---
add_executable(cmapd_tests
        test_map_class.cpp
        test_map_instance_class.cpp
        test_distances.cpp
        test_multi_a_star.cpp
        test_generation_instances.cpp
        test_cbs.cpp
        test_pp.cpp
        test_ortools.cpp
        ${CMAKE_SOURCE_DIR}/src/ambient/AmbientMap.cpp
        ${CMAKE_SOURCE_DIR}/src/ambient/AmbientMapInstance.cpp
        ${CMAKE_SOURCE_DIR}/src/generation/generate_instances.cpp)
target_include_directories(cmapd_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cmapd_tests
        PRIVATE
        distances
        multi_a_star
        path_finders
        task_assigner
        Catch2::Catch2WithMain
        fmt::fmt)

# --- unit tests ---
catch_discover_tests(cmapd_tests)

# --- integration tests ---
SET(instances_out_dir instances)

add_test(NAME generate_instances_integration
        COMMAND cmapd --generate -na 2 -nt 2 -ni 3 --instances-output ${instances_out_dir} data/map_5.txt)
set_tests_properties(generate_instances_integration PROPERTIES
        PASS_REGULAR_EXPRESSION
        ".*\nDone"
        FIXTURE_SETUP integration)

add_test(NAME pp_test_integration
        COMMAND cmapd --evaluate ${instances_out_dir} -c 1 -s PP data/map_5.txt)
set_tests_properties(pp_test_integration PROPERTIES
        PASS_REGULAR_EXPRESSION
        "(.|\n)*TOTAL TIME:.*\nMEAN TIME:.*"
        FIXTURE_REQUIRED integration)

add_test(NAME cbs_test_integration
        COMMAND cmapd --evaluate ${instances_out_dir} -c 1 -s CBS data/map_5.txt)
set_tests_properties(cbs_test_integration PROPERTIES
        PASS_REGULAR_EXPRESSION
        "(.|\n)*TOTAL TIME:.*\nMEAN TIME:.*"
        FIXTURE_REQUIRED integration)

add_test(NAME cleanup
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${instances_out_dir})
set_tests_properties(cleanup PROPERTIES
        FIXTURE_CLEANUP integration)

# target "test" for automated tests automatically added by cmake

# compiler warnings for tests

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(cmapd_tests PUBLIC -Wall -Wpedantic -Wextra -Werror)
endif ()