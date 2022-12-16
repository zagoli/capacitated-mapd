//
// Created by Jacopo on 18/10/2022.
//
#include <catch2/catch_test_macros.hpp>
#include "distances/distances.h"

namespace {

using namespace cmapd;

TEST_CASE("manhattan distance", "[distances]") {
    // diagonal distance
    Point first{0, 0}, second{1, 1};
    REQUIRE(manhattan_distance(first, second) == 2);
    // distance from self
    REQUIRE(manhattan_distance(second, second) == 0);
    // vertical distance
    first = {0, 0};
    second = {10, 0};
    REQUIRE(manhattan_distance(first, second) == 10);
    // horizontal distance
    first = {0, 0};
    second = {0, 10};
    REQUIRE(manhattan_distance(first, second) == 10);
}

TEST_CASE("compute h-table", "[distances]") {
    AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};
    // check h-table has all the correct entries
    REQUIRE(std::ssize(instance.h_table()) == 12);
    REQUIRE(std::ssize(instance.h_table().at({1, 0})) == 6);
    // distance from (1,0) to (1,2)
    REQUIRE(instance.h_table().at({1, 0}).at({1, 2}) == 2);
    // distance from (1,4) to (3,1)
    REQUIRE(instance.h_table().at({1, 4}).at({3, 1}) == 5);
    // distance from (1,1) to (3,1)
    REQUIRE(instance.h_table().at({1, 1}).at({3, 1}) == 2);
    // distance from (1,3) to (3,3)
    REQUIRE(instance.h_table().at({1, 3}).at({3, 3}) == 2);
    // distance from (1,1) to (3,3)
    REQUIRE(instance.h_table().at({1, 1}).at({3, 3}) == 4);
    // distance from (1,1) to (0,1) -> not present because (0,1) isn't a point of interest
    REQUIRE_THROWS(instance.h_table().at({1, 1}).at({0, 1}));
    // distance from (0,2) to (3,1) -> not present because (0,2) is a wall
    REQUIRE_THROWS(instance.h_table().at({0, 2}).at({3, 1}));
}

TEST_CASE("compute h-distance", "[distances]") {
    AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};

    Point location{1, 1};
    std::vector<Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
    int h_value = multi_a_star::compute_h_value(location, 0, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 6);

    h_value = multi_a_star::compute_h_value(location, 1, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 6);

    location = {2, 1};
    h_value = multi_a_star::compute_h_value(location, 0, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 7);

    h_value = multi_a_star::compute_h_value(location, 1, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 5);
}

}  // namespace