//
// Created by dade on 08/11/22.
//
#include <catch2/catch_test_macros.hpp>

#include "CmapdSolution.h"
#include "distances/distances.h"
#include "ortools/ortools.h"
#include "path_finders/pp.h"
#include "path_finders_utils.h"

namespace {

using namespace cmapd;

TEST_CASE("pp", "[pp]") {
    const AmbientMapInstance instance{"data/instance_2.txt", "data/map_2.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 1);

    std::vector<path_t> final_temp_paths = pp::pp(instance, goal_sequences).paths;
    REQUIRE_NOTHROW(are_valid_routes(final_temp_paths));
}

TEST_CASE("pp_3", "[pp]") {
    const AmbientMapInstance instance{"data/instance_3.txt", "data/map_3.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 2);

    std::vector<path_t> final_temp_paths = pp::pp(instance, goal_sequences).paths;
    REQUIRE_NOTHROW(are_valid_routes(final_temp_paths));
}

TEST_CASE("pp_4", "[pp]") {
    const AmbientMapInstance instance{"data/instance_4.txt", "data/map_4.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 4);

    std::vector<path_t> final_temp_paths = pp::pp(instance, goal_sequences).paths;
    REQUIRE_NOTHROW(are_valid_routes(final_temp_paths));
}
}  // namespace