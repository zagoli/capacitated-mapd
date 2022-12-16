//
// Created by Jacopo on 18/10/2022.
//
#include <catch2/catch_test_macros.hpp>

#include "distances/distances.h"

namespace {

using namespace cmapd;

TEST_CASE("compute h-table", "[distances]") {
    SECTION("Short version") {
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
    SECTION("Long version") {
        AmbientMapInstance instance{"data/instance_5.txt", "data/map_5.txt"};
        REQUIRE(std::ssize(instance.h_table().at({1, 0})) == 20);
    }
}

TEST_CASE("compute h-distance", "[distances]") {
    AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};

    Point location{1, 1};
    std::vector<Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
    int h_value = multi_a_star::compute_h_value(location, 0, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 8);

    h_value = multi_a_star::compute_h_value(location, 1, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 6);

    location = {2, 1};
    h_value = multi_a_star::compute_h_value(location, 0, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 9);

    h_value = multi_a_star::compute_h_value(location, 1, instance.h_table(), goal_sequence);
    REQUIRE(h_value == 5);
}

}  // namespace