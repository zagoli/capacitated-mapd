//
// Created by Jacopo on 18/10/2022.
//
#include <catch2/catch_test_macros.hpp>
#include "distances/distances.h"

namespace distanceTest {
TEST_CASE("test manhattan distance", "[distances]") {
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

TEST_CASE("test compute h-table", "[distances]") {
    cmapd::AmbientMapInstance instance {"data/test_instance.txt", "data/test_map.txt"};
    cmapd::h_table_t h_table = cmapd::compute_h_table(instance, cmapd::manhattan_distance);
    // distance from (1,4) to (3,1)
    REQUIRE(h_table.at({1,4}).at({3,1}) == 5);
    // distance from (1,1) to (3,1)
    REQUIRE(h_table.at({1,1}).at({3,1}) == 2);
    // distance from (1,3) to (3,3)
    REQUIRE(h_table.at({1,3}).at({3,3}) == 2);
    // distance from (1,1) to (3,3)
    REQUIRE(h_table.at({1,1}).at({3,3}) == 4);
    // distance from (1,1) to (0,1) -> not present because (0,1) isn't a point of interest
    REQUIRE_THROWS(h_table.at({1,1}).at({0,1}));
    // distance from (0,2) to (3,1) -> not present because (0,2) is a wall
    REQUIRE_THROWS(h_table.at({0,2}).at({3,1}));
}

}  // namespace distanceTest