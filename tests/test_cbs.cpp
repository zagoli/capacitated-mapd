//
// Created by Jacopo on 02/11/2022.
//
#include <catch2/catch_test_macros.hpp>

#include "CmapdSolution.h"
#include "distances/distances.h"
#include "path_finders/Node.h"
#include "path_finders/cbs.h"
#include "path_finders_utils.h"

namespace {

TEST_CASE("cbs node", "[cbs]") {
    const std::vector<cmapd::path_t> goal_sequences_1
        = {{{1, 1}, {1, 2}, {3, 2}}, {{1, 3}, {3, 1}, {3, 3}}};
    const std::vector<cmapd::path_t> goal_sequences_2 = {{{3, 0}, {3, 3}}, {{3, 3}, {3, 1}}};
    const cmapd::AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};
    cmapd::cbs::Node node1{instance, goal_sequences_1};
    cmapd::cbs::Node node2{instance, goal_sequences_2};

    SECTION("Paths") {
        REQUIRE(std::ssize(node1.get_paths()) == 2);
        REQUIRE(node1.lengths() == std::vector{6, 7});
        REQUIRE(node1.makespan() == 7);

        REQUIRE(std::ssize(node2.get_paths()) == 2);
        REQUIRE(node2.lengths() == std::vector{4, 3});
        REQUIRE(node2.makespan() == 4);
    }
    SECTION("Conflict") {
        cmapd::Conflict expected_conflict{.first_agent = 0,
                                          .second_agent = 1,
                                          .timestep = 1,
                                          .first_position = {1, 2},
                                          .second_position = {1, 2},
                                          .type = cmapd::ConflictType::VERTEX};
        REQUIRE(node1.first_conflict().value() == expected_conflict);

        expected_conflict = {.first_agent = 0,
                             .second_agent = 1,
                             .timestep = 2,
                             .first_position = {3, 1},
                             .second_position = {3, 2},
                             .type = cmapd::ConflictType::EDGE};
        REQUIRE(node2.first_conflict().value() == expected_conflict);
    }
}

TEST_CASE("simple cbs search", "[cbs]") {
    using namespace cmapd;
    AmbientMapInstance instance{"data/instance_2.txt", "data/map_2.txt"};
    std::vector<path_t> goal_sequences{{{1, 1}, {2, 2}, {3, 1}}};
    CmapdSolution solution{cbs::cbs(instance, goal_sequences)};
    REQUIRE(solution.cost == 5);
    REQUIRE(solution.makespan == 5);
    REQUIRE(solution.paths.size() == 1);
    const auto path = solution.paths.at(0);
    REQUIRE(*path.cbegin() == Point{1, 1});
    REQUIRE(path.back() == Point{3, 1});
    REQUIRE(std::find(path.cbegin(), path.cend(), Point{2, 2}) != path.cend());
}

TEST_CASE("medium cbs search", "[cbs]") {
    using namespace cmapd;
    AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};
    std::vector<path_t> goal_sequences{{{1, 1}, {1, 2}, {3, 2}}, {{1, 3}, {3, 1}, {3, 3}}};
    CmapdSolution solution{cbs::cbs(instance, goal_sequences)};
    REQUIRE(solution.paths.size() == 2);
    REQUIRE(solution.cost == 14);
    REQUIRE(solution.makespan == 7);

    const auto path_1 = solution.paths[0];
    REQUIRE(*path_1.cbegin() == Point{1, 1});
    REQUIRE(path_1.back() == Point{3, 2});
    REQUIRE(std::find(path_1.cbegin(), path_1.cend(), Point{1, 2}) != path_1.cend());

    const auto path_2 = solution.paths[1];
    REQUIRE(*path_2.cbegin() == Point{1, 3});
    REQUIRE(path_2.back() == Point{3, 3});
    REQUIRE(std::find(path_2.cbegin(), path_2.cend(), Point{3, 1}) != path_2.cend());

    REQUIRE_NOTHROW(solution.paths);
}

TEST_CASE("advanced cbs search", "[cbs]") {
    using namespace cmapd;
    AmbientMapInstance instance{"data/instance_5.txt", "data/map_5.txt"};
    std::vector<path_t> goal_sequences{{{1, 1}, {1, 2}, {17, 5}, {15, 5}, {7, 19}},
                                       {{19, 1}, {13, 29}, {15, 22}, {9, 8}, {9, 16}},
                                       {{1, 33}, {5, 13}, {15, 32}, {11, 11}, {15, 19}},
                                       {{19, 33}, {17, 26}, {1, 8}, {2, 29}, {9, 4}}};

    CmapdSolution solution{cbs::cbs(instance, goal_sequences)};

    REQUIRE(solution.paths.size() == 4);

    REQUIRE(*solution.paths[0].cbegin() == Point{1, 1});
    REQUIRE(solution.paths[0].back() == Point{7, 19});

    REQUIRE(*solution.paths[1].cbegin() == Point{19, 1});
    REQUIRE(solution.paths[1].back() == Point{9, 16});

    REQUIRE(*solution.paths[2].cbegin() == Point{1, 33});
    REQUIRE(solution.paths[2].back() == Point{15, 19});

    REQUIRE(*solution.paths[3].cbegin() == Point{19, 33});
    REQUIRE(solution.paths[3].back() == Point{9, 4});

    REQUIRE_NOTHROW(are_valid_routes(solution.paths));
}

}  // namespace