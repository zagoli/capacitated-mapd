//
// Created by Jacopo on 24/10/2022.
//

#include <catch2/catch_test_macros.hpp>

#include "a_star/Node.h"

namespace multi_a_star_test {

std::vector<cmapd::Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
cmapd::AmbientMapInstance instance{"data/test_instance.txt", "data/test_map.txt"};
cmapd::h_table_t h_table{cmapd::compute_h_table(instance, cmapd::manhattan_distance)};

TEST_CASE("A* node children", "[multi A*]") {
    cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
    std::vector<cmapd::multi_a_star::Node> children{node.get_children(instance)};
    REQUIRE(std::ssize(children) == 2l);

    cmapd::multi_a_star::Node node1{{1, 1}, h_table, goal_sequence};
    children = node1.get_children(instance);
    REQUIRE(std::ssize(children) == 4l);

    cmapd::multi_a_star::Node node2{{2, 1}, h_table, goal_sequence};
    children = node2.get_children(instance);
    REQUIRE(std::ssize(children) == 3l);
}

TEST_CASE("A* path", "[multi A*]") {
    cmapd::multi_a_star::Node parent{{1, 0}, h_table, goal_sequence};
    cmapd::multi_a_star::Node child1{{1, 1}, parent, h_table, goal_sequence};
    cmapd::multi_a_star::Node child2{{1, 2}, child1, h_table, goal_sequence};
    cmapd::multi_a_star::Node child3{{1, 3}, child2, h_table, goal_sequence};
    cmapd::multi_a_star::Node child4{{2, 3}, child3, h_table, goal_sequence};
    // final path
    auto path = child4.get_path();
    REQUIRE(std::ssize(path) == 5);
    REQUIRE(path[0] == cmapd::Point{1, 0});
    REQUIRE(path[1] == cmapd::Point{1, 1});
    REQUIRE(path[2] == cmapd::Point{1, 2});
    REQUIRE(path[3] == cmapd::Point{1, 3});
    REQUIRE(path[4] == cmapd::Point{2, 3});
    // intermediate path
    path = child2.get_path();
    REQUIRE(std::ssize(path) == 3);
    REQUIRE(path[0] == cmapd::Point{1, 0});
    REQUIRE(path[1] == cmapd::Point{1, 1});
    REQUIRE(path[2] == cmapd::Point{1, 2});
}

}  // namespace multi_a_star_test