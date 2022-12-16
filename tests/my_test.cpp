#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CmapdSolution.h"
#include "ambient/AmbientMapInstance.h"
#include "ortools/ortools.h"
#include "path_finders/cbs.h"
#include "path_finders/pp.h"

namespace {

void print_solution(const cmapd::CmapdSolution& solution) {
    for (int i = 0; i < std::ssize(solution.paths); ++i) {
        std::cout << "Agent " << i << ": [";
        const char* padding = "";
        for (const auto& point : solution.paths[i]) {
            std::cout << padding << point;
            padding = ", ";
        }
        std::cout << "]" << std::endl;
    }
}

TEST_CASE("my test") {
    using namespace cmapd;
    std::string instance_path = "data/instances/instance_7.txt";
    AmbientMapInstance instance{instance_path, "data/map_1.txt"};

    std::vector<path_t> goal_sequences{assign_tasks(instance, 2)};
    auto pp_sol = pp::pp(instance, goal_sequences);
    std::cout << "PRIORITIZED PLANNING\n";
    print_solution(pp_sol);
    auto cbs_sol = cbs::cbs(instance, goal_sequences);
    std::cout << "CBS\n";
    print_solution(cbs_sol);
    REQUIRE(cbs_sol.cost <= pp_sol.cost);
}

}  // namespace
