//
// Created by dade on 07/11/22.
//
#include <catch2/catch_test_macros.hpp>

#include "distances/distances.h"
#include "ortools/ortools.h"

namespace {

using namespace cmapd;

TEST_CASE("simple ortools test", "[ortools]") {
    const AmbientMapInstance instance{"data/instance_2.txt", "data/map_2.txt"};

    std::vector<path_t> goal_sequences = assign_tasks(instance, 1);
    REQUIRE(goal_sequences.size() == 1);
    std::vector valid_path{Point{1, 1}, Point{2, 2}, Point{3, 1}};
    REQUIRE(goal_sequences.at(0) == valid_path);
}

TEST_CASE("medium ortools test", "[ortools]") {
    const AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};

    std::vector<path_t> goal_sequences = assign_tasks(instance, 1);

    REQUIRE(goal_sequences.size() == 2);
    std::vector valid_path_0{Point{1, 1}, Point{3, 1}, Point{3, 3}};
    std::vector valid_path_1{Point{1, 3}, Point{1, 2}, Point{3, 2}};
    REQUIRE(goal_sequences.at(0) == valid_path_0);
    REQUIRE(goal_sequences.at(1) == valid_path_1);
}

TEST_CASE("advanced ortools test", "[ortools]") {
    const AmbientMapInstance instance{"data/instance_3.txt", "data/map_3.txt"};

    std::vector<path_t> goal_sequences = assign_tasks(instance, 2);

    REQUIRE(goal_sequences.size() == 3);
    std::vector valid_path_0{Point{0, 0},
                             Point{0, 1},
                             Point{0, 2},
                             Point{0, 3},
                             Point{0, 4},
                             Point{1, 4},
                             Point{1, 3},
                             Point{1, 2},
                             Point{1, 1}};
    std::vector valid_path_1{Point{0, 9}, Point{1, 8}, Point{1, 9}};
    std::vector valid_path_2{Point{9, 9}, Point{8, 9}, Point{7, 9}, Point{7, 8}, Point{7, 7}};
    REQUIRE(goal_sequences.at(0) == valid_path_0);
    REQUIRE(goal_sequences.at(1) == valid_path_1);
    REQUIRE(goal_sequences.at(2) == valid_path_2);
}

}  // namespace
