//
// Created by dade on 14/10/22.
//

#include <ambient/AmbientMap.h>

#include <catch2/catch_test_macros.hpp>

namespace AmbientMapTest {
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

    REQUIRE(map.get_rows_number() == 5);
    REQUIRE(map.get_columns_number() == 5);
}

TEST_CASE("test if a point is inside the map", "[is_inside_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_FALSE(map.is_inside_position(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_inside_position(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(map.is_inside_position(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 3;
    REQUIRE_FALSE(map.is_inside_position(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_inside_position(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_inside_position(invalid_point));

    cmapd::Point valid_point{1, 2};
    REQUIRE(map.is_inside_position(valid_point));
    valid_point.row = 2;
    valid_point.col = 1;
    REQUIRE(map.is_inside_position(valid_point));
}

TEST_CASE("test if a point is valid in a map", "[is_valid_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 3;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 5;
    invalid_point.col = 5;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = 1;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));

    cmapd::Point valid_point{1, 2};
    REQUIRE(map.is_valid_position(valid_point));
}

TEST_CASE("test to ravel a point in a map", "[ravel_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    cmapd::Point invalid_point{-1, -1};
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);
    invalid_point.row = 5;
    invalid_point.col = 3;
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);
    invalid_point.row = 3;
    invalid_point.col = 5;
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);
    invalid_point.row = 5;
    invalid_point.col = 5;
    REQUIRE_THROWS_AS(map.ravel(invalid_point), std::runtime_error);

    cmapd::Point valid_point{1, 2};
    REQUIRE(map.ravel(valid_point) == 7);
    valid_point.row = 2;
    valid_point.col = 1;
    REQUIRE(map.ravel(valid_point) == 11);
}

TEST_CASE("test to unravel a point in a map", "[unravel_point_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    REQUIRE_THROWS_AS(map.unravel(-1), std::runtime_error);
    int64_t out_of_bound_index{map.get_rows_number() * map.get_columns_number()};
    REQUIRE_THROWS_AS(map.unravel(out_of_bound_index), std::runtime_error);

    cmapd::Point valid_point{map.get_rows_number() - 1, map.get_columns_number() - 1};
    int64_t inside_index{map.get_rows_number() * map.get_columns_number() - 1};

    REQUIRE(map.unravel(inside_index) == valid_point);
    valid_point.row = 0;
    valid_point.col = 0;
    REQUIRE(map.unravel(0) == valid_point);
    valid_point = {1, 2};
    REQUIRE(map.unravel(7) == valid_point);
}

TEST_CASE("test if to_string return the correct map string", "[to_string_method_map]") {
    std::filesystem::path path_to_map{"data/map_1.txt"};
    AmbientMap map{path_to_map};

    REQUIRE(map.to_string() == "#####\n OOO \n# # #\n OOO \n#####\n");
}
}  // namespace AmbientMapTest