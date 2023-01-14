message(DEBUG "processing src folder")

# --- Source code ----

# distances library
add_library(distances STATIC
        distances/distances.cpp)
target_include_directories(distances PRIVATE ${CMAKE_SOURCE_DIR}/src)

# multi_a_star library
add_library(multi_a_star STATIC
        a_star/Node.cpp
        a_star/Frontier.cpp
        a_star/multi_a_star.cpp)
target_include_directories(multi_a_star PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(multi_a_star PRIVATE distances)

# cbs and pp library
add_library(path_finders STATIC
        path_finders/Node.cpp
        path_finders/cbs.cpp
        path_finders/pp.cpp)
target_include_directories(path_finders PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(path_finders PRIVATE multi_a_star)

# task assignment library
add_library(task_assigner STATIC
        ortools/ortools.cpp)
target_include_directories(task_assigner PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(task_assigner PRIVATE ortools::ortools)

# program
add_executable(cmapd
        main.cpp
        ambient/AmbientMap.cpp
        ambient/AmbientMapInstance.cpp
        generation/generate_instances.cpp
        ConstraintsContainer.cpp)
target_include_directories(cmapd PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/timer)
target_link_libraries(cmapd PRIVATE
        task_assigner
        path_finders
        argparse::argparse
        fmt::fmt)

# compilers warning for main executable

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(cmapd PUBLIC -Wall -Wpedantic -Wextra -Werror)
endif ()