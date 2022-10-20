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
}  // namespace distanceTest