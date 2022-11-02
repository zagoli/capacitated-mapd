//
// Created by dade on 21/10/22.
//

#include <ambient/AmbientMapInstance.h>

#include <catch2/catch_test_macros.hpp>

namespace AmbientMapInstanceTest {
using cmapd::AmbientMapInstance;

TEST_CASE("test assign instance", "[instantiation_instance]") {
    std::filesystem::path path_to_map{"data/test_maps.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};

    REQUIRE_THROWS_AS(AmbientMapInstance(path_to_instance, path_to_map), std::runtime_error);

    path_to_map = "data/test_map.txt";
    REQUIRE_NOTHROW(AmbientMapInstance(path_to_instance, path_to_map));
}

TEST_CASE("test get number of rows and columns of a instance", "[rows_and_columns_instance]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};
    AmbientMapInstance instance{path_to_instance, path_to_map};

    REQUIRE(instance.get_rows_number() == 5);
    REQUIRE(instance.get_columns_number() == 5);
}

TEST_CASE("test get number of agents and tasks of a instance", "[agents_and_tasks_instance]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};
    AmbientMapInstance instance{path_to_instance, path_to_map};

    REQUIRE(instance.get_num_agents() == 2);
    REQUIRE(instance.get_num_tasks() == 2);
}

TEST_CASE("test if a point is valid in a instance", "[is_valid_point_instance]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};
    AmbientMapInstance instance{path_to_instance, path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));
    invalid_point.row = 6;
    invalid_point.col = 3;
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 6;
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));
    invalid_point.row = 6;
    invalid_point.col = 6;
    REQUIRE_FALSE(instance.is_valid_position(invalid_point));

    cmapd::Point valid_point{2, 1};
    REQUIRE(instance.is_valid_position(valid_point));
}

TEST_CASE("test get list of all tasks of a instance", "[tasks_instance]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};
    AmbientMapInstance instance{path_to_instance, path_to_map};

    std::vector<std::pair<cmapd::Point, cmapd::Point>> expected_tasks{};
    expected_tasks.emplace_back(cmapd::Point{1, 2}, cmapd::Point{3, 2});
    expected_tasks.emplace_back(cmapd::Point{3, 1}, cmapd::Point{3, 3});

    REQUIRE(expected_tasks == instance.get_tasks());
}

TEST_CASE("test if to_string return the correct instance string", "[to_string_method_instance]") {
    std::filesystem::path path_to_map{"data/test_map.txt"};
    std::filesystem::path path_to_instance{"data/test_instance.txt"};
    AmbientMapInstance instance{path_to_instance, path_to_map};

    REQUIRE(instance.to_string() == "#####\n ata \n# # #\n ttt \n#####\n");
}
}
