//
// Created by Jacopo on 24/10/2022.
//

#include <catch2/catch_test_macros.hpp>

#include "a_star/Frontier.h"
#include "a_star/Node.h"
#include "a_star/multi_a_star.h"

namespace {
using namespace cmapd;
const std::vector<Point> goal_sequence = {{1, 2}, {3, 2}, {3, 1}, {3, 3}};
const AmbientMapInstance instance{"data/instance_1.txt", "data/map_1.txt"};

TEST_CASE("Multi A* node equality", "[multi A*]") {
    multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
    multi_a_star::Node node1{{1, 0}, instance.h_table(), goal_sequence};
    multi_a_star::Node node2{{1, 0}, node, instance.h_table(), goal_sequence};
    multi_a_star::Node node3{{1, 1}, instance.h_table(), goal_sequence};
    multi_a_star::Node node4{{1, 1}, node, instance.h_table(), goal_sequence};
    REQUIRE(node == node1);
    REQUIRE(node != node2);
    REQUIRE(node != node3);
    REQUIRE(node != node4);
}

TEST_CASE("Multi A* node children", "[multi A*]") {
    multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
    std::vector<multi_a_star::Node> children{node.get_children(instance)};
    REQUIRE(std::ssize(children) == 2l);

    multi_a_star::Node node1{{1, 1}, instance.h_table(), goal_sequence};
    children = node1.get_children(instance);
    REQUIRE(std::ssize(children) == 4l);

    multi_a_star::Node node2{{2, 1}, instance.h_table(), goal_sequence};
    children = node2.get_children(instance);
    REQUIRE(std::ssize(children) == 3l);
}

TEST_CASE("Multi A* path", "[multi A*]") {
    multi_a_star::Node parent{{1, 0}, instance.h_table(), goal_sequence};
    multi_a_star::Node child1{{1, 1}, parent, instance.h_table(), goal_sequence};
    multi_a_star::Node child2{{1, 2}, child1, instance.h_table(), goal_sequence};
    multi_a_star::Node child3{{1, 3}, child2, instance.h_table(), goal_sequence};
    multi_a_star::Node child4{{2, 3}, child3, instance.h_table(), goal_sequence};
    // final path
    auto path = child4.get_path();
    path_t expected_path{{1, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 3}};
    REQUIRE(std::ssize(path) == 5);
    REQUIRE(path == expected_path);
    // intermediate path
    path = child2.get_path();
    expected_path = {{1, 0}, {1, 1}, {1, 2}};
    REQUIRE(std::ssize(path) == 3);
    REQUIRE(path == expected_path);
}

TEST_CASE("Multi A* Frontier", "[multi A*]") {
    SECTION("Empty frontier, push and pop") {
        multi_a_star::Frontier frontier{};
        REQUIRE(frontier.empty());
        multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
        frontier.push(node);
        multi_a_star::Node node1{frontier.pop()};
        REQUIRE(frontier.empty());
        REQUIRE(node == node1);
    }
    SECTION("Pop node with smaller f-value") {
        multi_a_star::Frontier frontier{};
        multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
        multi_a_star::Node node1{{1, 1}, instance.h_table(), goal_sequence};
        multi_a_star::Node node2{{1, 2}, instance.h_table(), goal_sequence};
        REQUIRE(node.get_f_value() > node1.get_f_value());
        REQUIRE(node1.get_f_value() > node2.get_f_value());
        frontier.push(node);
        frontier.push(node2);
        frontier.push(node1);
        multi_a_star::Node res = frontier.pop();
        REQUIRE(res == node2);
        REQUIRE_FALSE(frontier.contains(node2));
    }
    SECTION("Contains Point") {
        multi_a_star::Frontier frontier{};
        multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
        multi_a_star::Node node1{{1, 1}, instance.h_table(), goal_sequence};
        frontier.push(node);
        REQUIRE(frontier.contains(node));
        REQUIRE_FALSE(frontier.contains(node1));
    }
    SECTION("Contains Point more expensive") {
        multi_a_star::Frontier frontier{};
        multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
        multi_a_star::Node node1{{1, 1}, instance.h_table(), goal_sequence};
        frontier.push(node);
        // doesn't contain Node
        REQUIRE_FALSE(frontier.contains_more_expensive(node1, 0));
        // contains Node but cheaper
        REQUIRE_FALSE(frontier.contains_more_expensive(node, 9));
        // contains Node more expensive
        REQUIRE(frontier.contains_more_expensive(node, 6));
    }
    SECTION("Replace a Node") {
        multi_a_star::Frontier frontier{};
        multi_a_star::Node node{{1, 0}, instance.h_table(), goal_sequence};
        multi_a_star::Node node1{{1, 1}, instance.h_table(), goal_sequence};
        // Replace node that's not in the frontier
        REQUIRE_THROWS(frontier.replace(node, node));
        // Simple replace
        frontier.push(node);
        frontier.replace(node, node1);
        REQUIRE(frontier.contains(node1));
        REQUIRE_FALSE(frontier.contains(node));
        // Find and replace
        frontier = {};  // reset the frontier
        frontier.push(node);
        auto optional_node = frontier.contains_more_expensive(node, 5);
        if (optional_node) {
            // replace node with node1
            frontier.replace(optional_node.value(), node1);
        }
        REQUIRE(frontier.contains(node1));
        REQUIRE_FALSE(frontier.contains(node));
    }
}

TEST_CASE("multi A* complete", "[multi A*]") {
    SECTION("No constraints") {
        std::vector<Point> goals{{1, 2}, {3, 3}};
        auto path{multi_a_star::multi_a_star(0, {1, 0}, goals, instance, {})};
        REQUIRE(std::ssize(path) == 6);
        REQUIRE(path.at(0) == Point{1, 0});  // starting point
        REQUIRE(path.at(3) == Point{1, 3});  // middle point
        REQUIRE(path.at(5) == Point{3, 3});  // ending point
        // another different path
        goals = {{3, 1}, {3, 3}};
        path = multi_a_star::multi_a_star(0, {1, 0}, goals, instance);
        REQUIRE(std::ssize(path) == 6);
        REQUIRE(path.at(0) == Point{1, 0});  // starting point
        REQUIRE(path.at(2) == Point{2, 1});  // middle point
        REQUIRE(path.at(5) == Point{3, 3});  // ending point
    }
    SECTION("Constraints") {
        std::vector<Point> goals{{3, 1}};
        std::vector<Constraint> constraints{
            {.agent = 0, .timestep = 2, .from_position = {1, 3}, .to_position = {2, 3}},
            {.agent = 1, .timestep = 2, .from_position = {1, 3}, .to_position = {1, 2}}};
        auto path{multi_a_star::multi_a_star(0, {1, 4}, goals, instance, constraints)};
        REQUIRE(std::ssize(path) == 6);
        path_t expected_path{{1, 4}, {1, 3}, {1, 2}, {1, 1}, {2, 1}, {3, 1}};
        REQUIRE(path == expected_path);
    }
    SECTION("Timeout") {
        AmbientMapInstance bad_instance{"data/instance_6.txt", "data/map_6.txt"};
        std::vector<Point> goals{{3, 0}, {3, 4}};
        REQUIRE_THROWS(multi_a_star::multi_a_star(0, {1, 1}, goals, bad_instance));
    }
}

}  // namespace