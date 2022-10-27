//
// Created by Jacopo on 25/10/2022.
//

#include "multi_a_star.h"

#include <algorithm>
#include <queue>
#include <set>

#include "Frontier.h"

namespace cmapd::multi_a_star {

std::vector<Point> multi_a_star(int agent,
                                Point start_location,
                                const std::vector<Point>& goal_sequence,
                                const AmbientMapInstance& map_instance,
                                const std::vector<Constraint>& constraints) {
    // frontier definition
    Frontier frontier;
    // explore set definition
    std::set<Point> explored;
    // generation of the h-table
    h_table_t h_table{compute_h_table(map_instance, manhattan_distance)};
    // generation of root node in the frontier
    frontier.push(Node{start_location, h_table, goal_sequence});
    // main loop
    while (!frontier.empty()) {
        // get top node
        auto top_node{frontier.pop()};
        // Update label
        if (top_node.get_location() == goal_sequence[top_node.get_label()]) {
            top_node.increment_label();
        }
        // Goal test
        if (top_node.get_label() == std::ssize(goal_sequence)) {
            return top_node.get_path();
        }
        // Remember that we visited this location
        explored.insert(top_node.get_location());
        // Populate queue
        for (const auto& child : top_node.get_children(map_instance)) {
            Constraint check_me{.agent = agent,
                                .timestep = child.get_g_value(),
                                .from_position = top_node.get_location(),
                                .to_position = child.get_location()};
            // check if child is constrained
            if (std::find(constraints.cbegin(), constraints.cend(), check_me)
                == constraints.cend()) {
                if (!explored.contains(child.get_location())
                    && !frontier.contains_point(child.get_location())) {
                    frontier.push(child);
                } else if (auto costly_child_opt = frontier.contains_point_more_expensive(
                               child.get_location(), child.get_f_value())) {
                    frontier.replace(costly_child_opt.value(), child);
                }
            }
        }
    }
    // No solution is found
    throw std::runtime_error("No solution found.");
}

}  // namespace cmapd::multi_a_star