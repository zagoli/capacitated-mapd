//
// Created by dade on 14/10/22.
//

#include <AmbientMap.h>
#include <catch2/catch_test_macros.hpp>

namespace AmbientMapTest {
using cmapd::AmbientMap;

TEST_CASE("test instantiation map", "[instantiation]") {
    
    std::filesystem::path path_to_map{"/home/dade/Desktop/capacitated-mapd/tests/test_maps.txt"};
    REQUIRE_THROWS_AS(AmbientMap(path_to_map), std::runtime_error);

    path_to_map = "/home/dade/Desktop/capacitated-mapd/tests/test_map.txt";
    REQUIRE_NOTHROW(AmbientMap(path_to_map));
}

TEST_CASE("test get number of rows and columns of a instantiated map", "[rows_and_columns]") {
    
    std::filesystem::path path_to_map{"/home/dade/Desktop/capacitated-mapd/tests/test_map.txt"};
    AmbientMap map {path_to_map};
    
    REQUIRE(map.get_rows() == 5);
    REQUIRE(map.get_columns() == 5);

}

TEST_CASE("test if a point is valid in a instantiated map", "[is_valid_point]") {
    
    std::filesystem::path path_to_map{"/home/dade/Desktop/capacitated-mapd/tests/test_map.txt"};
    AmbientMap map {path_to_map};
    
    Point invalid_point{};
    invalid_point.row = -1;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 2;
    invalid_point.col = -1;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = -1;
    invalid_point.col = 2;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 6;
    invalid_point.col = 3;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 3;
    invalid_point.col = 6;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    invalid_point.row = 6;
    invalid_point.col = 6;
    REQUIRE_FALSE(map.is_valid_position(invalid_point));
    
    Point valid_point{};
    valid_point.row = 2;
    valid_point.col = 2;
    REQUIRE(map.is_valid_position(valid_point));
}

TEST_CASE("test if to_string return the correct string", "[to_string_method]") {
    
    std::filesystem::path path_to_map{"/home/dade/Desktop/capacitated-mapd/tests/test_map.txt"};
    AmbientMap map {path_to_map};
    
    REQUIRE(map.to_string() == "#####\n OOO \n# # #\n OOO \n#####\n");
}
}