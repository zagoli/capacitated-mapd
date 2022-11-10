//
// Created by Jacopo on 02/11/2022.
//
#include <catch2/catch_test_macros.hpp>

#include "distances/distances.h"
#include "path_finders/Node.h"

namespace cbs_test {

const std::vector<cmapd::path_t> goal_sequences_1
    = {{{1, 1}, {1, 2}, {3, 2}}, {{1, 3}, {3, 1}, {3, 3}}};
const std::vector<cmapd::path_t> goal_sequences_2 = {{{3, 0}, {3, 3}}, {{3, 3}, {3, 1}}};
const cmapd::AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};

TEST_CASE("cbs node", "[cbs]") {
    cmapd::cbs::Node node1{instance, goal_sequences_1};
    cmapd::cbs::Node node2{instance, goal_sequences_2};
    SECTION("Paths") {
        REQUIRE(std::ssize(node1.get_paths()) == 2);
        REQUIRE(node1.get_lengths() == std::vector{6, 7});
        REQUIRE(node1.get_makespan() == 7);

        REQUIRE(std::ssize(node2.get_paths()) == 2);
        REQUIRE(node2.get_lengths() == std::vector{4, 3});
        REQUIRE(node2.get_makespan() == 4);
    }
    SECTION("Conflict") {
        cmapd::Conflict expected_conflict{.first_agent = 0,
                                          .second_agent = 1,
                                          .timestep = 1,
                                          .first_position = {1, 2},
                                          .second_position = {1, 2},
                                          .type = cmapd::ConflictType::VERTEX};
        REQUIRE(node1.get_first_conflict().value() == expected_conflict);

        expected_conflict = {.first_agent = 0,
                             .second_agent = 1,
                             .timestep = 2,
                             .first_position = {3, 1},
                             .second_position = {3, 2},
                             .type = cmapd::ConflictType::EDGE};
        REQUIRE(node2.get_first_conflict().value() == expected_conflict);
    }
}

}  // namespace cbs_test