//
// Created by dade on 08/11/22.
//
#include <catch2/catch_test_macros.hpp>

#include "CmapdSolution.h"
#include "distances/distances.h"
#include "ortools/ortools.h"
#include "path_finders/pbs.h"
#include "path_finders_utils.h"

namespace pbs_test {

using namespace cmapd;

TEST_CASE("pbs", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_2.txt", "data/map_2.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 1);

    std::vector<path_t> final_temp_paths = pbs::pbs(instance, goal_sequences).paths;
}

TEST_CASE("pbs_3", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_3.txt", "data/map_3.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 2);

    std::vector<path_t> final_temp_paths = pbs::pbs(instance, goal_sequences).paths;
}

TEST_CASE("pbs_4", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_4.txt", "data/map_4.txt"};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 4);

    std::vector<path_t> final_temp_paths = pbs::pbs(instance, goal_sequences).paths;
    REQUIRE_NOTHROW(are_valid_routes(final_temp_paths));
}
}  // namespace pbs_test