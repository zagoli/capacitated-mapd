//
// Created by Jacopo on 24/10/2022.
//

#include <catch2/catch_test_macros.hpp>

#include "a_star/Frontier.h"
#include "a_star/Node.h"

namespace multi_a_star_test {

std::vector<cmapd::Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
cmapd::AmbientMapInstance instance{"data/test_instance.txt", "data/test_map.txt"};
cmapd::h_table_t h_table{cmapd::compute_h_table(instance, cmapd::manhattan_distance)};

TEST_CASE("Multi A* node children", "[multi A*]") {
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

TEST_CASE("Multi A* path", "[multi A*]") {
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

TEST_CASE("Multi A* Frontier", "[multi A*]") {
    SECTION("Empty frontier, push and pop") {
        cmapd::multi_a_star::Frontier frontier{};
        REQUIRE(frontier.empty());
        cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
        frontier.push(node);
        cmapd::multi_a_star::Node node1{frontier.pop()};
        REQUIRE(frontier.empty());
        REQUIRE(node == node1);
    }
    SECTION("Pop node with smaller f-value") {
        cmapd::multi_a_star::Frontier frontier{};
        cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
        cmapd::multi_a_star::Node node1{{1, 1}, h_table, goal_sequence};
        cmapd::multi_a_star::Node node2{{1, 2}, h_table, goal_sequence};
        REQUIRE(node.get_f_value() > node1.get_f_value());
        REQUIRE(node1.get_f_value() > node2.get_f_value());
        frontier.push(node);
        frontier.push(node2);
        frontier.push(node1);
        cmapd::multi_a_star::Node res = frontier.pop();
        REQUIRE(res == node2);
        REQUIRE_FALSE(frontier.contains_point({1, 2}));
    }
    SECTION("Contains Point") {
        cmapd::multi_a_star::Frontier frontier{};
        cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
        cmapd::multi_a_star::Node node1{{1, 1}, h_table, goal_sequence};
        frontier.push(node);
        REQUIRE(frontier.contains_point(node.get_location()));
        REQUIRE_FALSE(frontier.contains_point(node1.get_location()));
    }
    SECTION("Contains Point more expensive") {
        cmapd::multi_a_star::Frontier frontier{};
        cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
        frontier.push(node);
        // doesn't contain point
        REQUIRE_FALSE(frontier.contains_point_more_expensive({1, 1}, 0));
        // contains Point but cheaper
        REQUIRE_FALSE(frontier.contains_point_more_expensive({1, 0}, 8));
        // contains Point more expensive
        REQUIRE(frontier.contains_point_more_expensive({1, 0}, 6));
    }
    SECTION("Replace a Node") {
        cmapd::multi_a_star::Frontier frontier{};
        cmapd::multi_a_star::Node node{{1, 0}, h_table, goal_sequence};
        cmapd::multi_a_star::Node node1{{1, 1}, h_table, goal_sequence};
        // Simple replace
        frontier.push(node);
        frontier.replace(node, node1);
        REQUIRE(frontier.contains_point({1, 1}));
        REQUIRE_FALSE(frontier.contains_point({1, 0}));
        // Find and replace
        frontier = {};  // reset the frontier
        frontier.push(node);
        auto optional_node = frontier.contains_point_more_expensive({1, 0}, 5);
        if (optional_node) {
            // replace node with node1
            frontier.replace(optional_node.value(), node1);
        }
        REQUIRE(frontier.contains_point({1, 1}));
        REQUIRE_FALSE(frontier.contains_point({1, 0}));
    }
}

}  // namespace multi_a_star_test