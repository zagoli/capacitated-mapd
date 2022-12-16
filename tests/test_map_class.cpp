//
// Created by dade on 14/10/22.
//

#include <ambient/AmbientMap.h>

#include <catch2/catch_test_macros.hpp>

namespace {
using cmapd::AmbientMap;

TEST_CASE("test assign map", "[instantiation_map]") {
    std::filesystem::path path_to_map{"data/test_maps.txt"};
    REQUIRE_THROWS_AS(AmbientMap(path_to_map), std::runtime_error);

    path_to_map = "data/map_wrong.txt";
    REQUIRE_THROWS_AS(AmbientMap(path_to_map), std::runtime_error);

    path_to_map = "data/map_1.txt";
    REQUIRE_NOTHROW(AmbientMap(path_to_map));
}

TEST_CASE("test get number of rows and columns of a map", "[rows_and_columns_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    REQUIRE(map.rows_number() == 5);
    REQUIRE(map.columns_number() == 5);
}

TEST_CASE("test if a point is inside the map", "[is_inside_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_FALSE(map.is_inside(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_inside(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(map.is_inside(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 3;
    REQUIRE_FALSE(map.is_inside(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_inside(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_inside(invalid_point));

    cmapd::Point valid_point{1, 2};
    REQUIRE(map.is_inside(valid_point));
    valid_point.row = 2;
    valid_point.col = 1;
    REQUIRE(map.is_inside(valid_point));
}

TEST_CASE("test if a point is valid in a map", "[is_valid_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 3;
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_valid(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = 1;
    REQUIRE_FALSE(map.is_valid(invalid_point));

    cmapd::Point valid_point{1, 2};
    REQUIRE(map.is_valid(valid_point));
}

TEST_CASE("test if to_string return the correct map string", "[to_string_method_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    REQUIRE(map.to_string() == "#####\n OOO \n# # #\n OOO \n#####\n");
}
}  // namespace