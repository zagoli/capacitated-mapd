//
// Created by dade on 30/10/22.
//
#include <ambient/AmbientMapInstance.h>

#include <catch2/catch_test_macros.hpp>

#include "generation/generate_instances.h"

namespace GenerateInstances {
using cmapd::generate_instances;

TEST_CASE("test generation instances", "[generate_instances]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    cmapd::AmbientMap map{path_to_map};

    int n_instances = 0;
    int n_agents = 0;
    int n_tasks = 0;

    REQUIRE_THROWS_AS(generate_instances(map, n_instances, n_agents, n_tasks), std::runtime_error);

    n_instances = 10;
    n_agents = 7;
    n_tasks = 0;
    REQUIRE_THROWS_AS(generate_instances(map, n_instances, n_agents, n_tasks), std::runtime_error);

    n_agents = 0;
    n_tasks = 4;
    REQUIRE_THROWS_AS(generate_instances(map, n_instances, n_agents, n_tasks), std::runtime_error);

    n_agents = 7;
    n_tasks = 4;
    REQUIRE_THROWS_AS(generate_instances(map, n_instances, n_agents, n_tasks), std::runtime_error);

    n_agents = 2;
    n_tasks = 2;
    REQUIRE_NOTHROW(generate_instances(map, n_instances, n_agents, n_tasks));

    std::vector<cmapd::AmbientMapInstance> instances
        = generate_instances(map, n_instances, n_agents, n_tasks);

    REQUIRE(static_cast<int>(instances.size()) == n_instances);
    REQUIRE(instances[0].get_num_agents() == 2);
    REQUIRE(instances[0].get_num_tasks() == 2);
}

}  // namespace GenerateInstances