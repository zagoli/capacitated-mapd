//
// Created by Jacopo on 18/10/2022.
//
#include <catch2/catch_test_macros.hpp>
#include "distances/distances.h"

namespace distanceTest {
TEST_CASE("manhattan distance", "[distances]") {
    // diagonal distance
    cmapd::Point first{0, 0}, second{1, 1};
    REQUIRE(cmapd::manhattan_distance(first, second) == 2);
    // distance from self
    REQUIRE(cmapd::manhattan_distance(second, second) == 0);
    // vertical distance
    first = {0, 0};
    second = {10, 0};
    REQUIRE(cmapd::manhattan_distance(first, second) == 10);
    // horizontal distance
    first = {0, 0};
    second = {0, 10};
    REQUIRE(cmapd::manhattan_distance(first, second) == 10);
}

TEST_CASE("compute h-table", "[distances]") {
    cmapd::AmbientMapInstance instance{"data/test_instance.txt", "data/test_map.txt"};
    cmapd::h_table_t h_table = cmapd::compute_h_table(instance, cmapd::manhattan_distance);
    // check h-table has all the correct entries
    REQUIRE(std::ssize(h_table) == 12);
    REQUIRE(std::ssize(h_table.at({1, 0})) == 4);
    // distance from (1,0) to (1,2)
    REQUIRE(h_table.at({1, 0}).at({1, 2}) == 2);
    // distance from (1,4) to (3,1)
    REQUIRE(h_table.at({1, 4}).at({3, 1}) == 5);
    // distance from (1,1) to (3,1)
    REQUIRE(h_table.at({1, 1}).at({3, 1}) == 2);
    // distance from (1,3) to (3,3)
    REQUIRE(h_table.at({1, 3}).at({3, 3}) == 2);
    // distance from (1,1) to (3,3)
    REQUIRE(h_table.at({1, 1}).at({3, 3}) == 4);
    // distance from (1,1) to (0,1) -> not present because (0,1) isn't a point of interest
    REQUIRE_THROWS(h_table.at({1, 1}).at({0, 1}));
    // distance from (0,2) to (3,1) -> not present because (0,2) is a wall
    REQUIRE_THROWS(h_table.at({0, 2}).at({3, 1}));
}

TEST_CASE("compute h-distance", "[distances]") {
    cmapd::AmbientMapInstance instance{"data/test_instance.txt", "data/test_map.txt"};
    cmapd::h_table_t h_table = cmapd::compute_h_table(instance, cmapd::manhattan_distance);

    cmapd::Point location{1, 1};
    std::vector<cmapd::Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
    int h_value = cmapd::multi_a_star::compute_h_value(location, 0, h_table, goal_sequence);
    REQUIRE(h_value == 6);

    h_value = cmapd::multi_a_star::compute_h_value(location, 1, h_table, goal_sequence);
    REQUIRE(h_value == 6);

    location = {2, 1};
    h_value = cmapd::multi_a_star::compute_h_value(location, 0, h_table, goal_sequence);
    REQUIRE(h_value == 7);

    h_value = cmapd::multi_a_star::compute_h_value(location, 1, h_table, goal_sequence);
    REQUIRE(h_value == 5);
}

}  // namespace distanceTest