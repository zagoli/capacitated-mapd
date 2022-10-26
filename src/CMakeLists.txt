message(STATUS "processing src folder")

# --- Source code ----
add_library(distances STATIC
        distances/distances.cpp)
target_include_directories(distances PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(MultiAStar STATIC
        a_star/Node.cpp
        a_star/Frontier.cpp
        a_star/multi_a_star.cpp)
target_include_directories(MultiAStar PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(MultiAStar PRIVATE distances)

add_executable(cmapd main.cpp AmbientMap.cpp AmbientMapInstance.cpp)
target_link_libraries(cmapd PRIVATE argparse::argparse fmt::fmt MultiAStar)