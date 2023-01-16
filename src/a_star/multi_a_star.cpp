/**
 * @file
 * @brief Contains the implementation of multi_a_star.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "a_star/multi_a_star.h"

#include <vector>

#include "Constraint.h"
#include "ConstraintsContainer.h"
#include "Point.h"
#include "a_star/Frontier.h"
#include "a_star/Node.h"
#include "custom_types.h"
#include "distances/distances.h"

namespace cmapd::multi_a_star {

/**
 * Check if an A* Node is present in the constraint list.
 * @param constraints The list of constraints.
 * @param agent The agent which we are checking.
 * @param child The node to be checked.
 * @param parent The parent of child.
 * @return true if child is constrained.
 */
bool is_constrained(const ConstraintsContainer& constraints,
                    int agent,
                    const Node& child,
                    const Point& from_position);

/**
 * Checks if a Node can be the last one of his path.
 * @param agent The agent we are computing the path for.
 * @param ending_node The candidate last Node.
 * @param constraints The constraint list.
 * @return true if there are no constraints on the location in ending_node after the current time.
 */
bool can_path_end_here(int agent, const Node& ending_node, const ConstraintsContainer& constraints);

/**
 * Check if a node is not already in the frontier and in the explored set.
 * @return true iff child is not in explored and frontier.
 */
bool is_child_novel(const Frontier& frontier, const std::vector<Node>& explored, const Node& child);

path_t multi_a_star(int agent,
                    Point start_location,
                    const path_t& goal_sequence,
                    const AmbientMapInstance& map_instance,
                    const ConstraintsContainer& constraints,
                    int timeout) {
    // compute timeout value
    if (timeout <= 0) {
        timeout = map_instance.rows_number() * map_instance.columns_number() * 10;
    }
    if (goal_sequence.empty()) {
        throw std::runtime_error{"[multiastar] the goal sequence is empty!"};
    }
    // frontier definition
    const int queues_reserved_space{
        compute_h_value(start_location, 0, map_instance.h_table(), goal_sequence) * 10};
    Frontier frontier(queues_reserved_space);
    // explored set definition
    std::vector<Node> explored;
    explored.reserve(queues_reserved_space);
    // generation of root node in the frontier
    frontier.push(Node{start_location, map_instance.h_table(), goal_sequence});
    // main loop
    while (!frontier.empty()) {
        // timeout operations
        if (timeout <= 0) {
            throw std::runtime_error("[multiastar] Timeout! For agent " + std::to_string(agent));
        } else {
            --timeout;
        }
        // get top node
        auto top_node{frontier.pop()};
        // Update label
        if (top_node.get_location() == goal_sequence[top_node.get_label()]) {
            top_node.increment_label();
        }
        // Goal test
        if (top_node.get_label() == std::ssize(goal_sequence)) {
            if (can_path_end_here(agent, top_node, constraints)) {
                return top_node.get_path();
            } else {
                // We wish to end the path here, but it's not possible.
                // As a result, the last goal location (this one) should not appear as visited, and
                // we must visit it again later!
                top_node.decrement_label();
            }
        }
        // Remember that we visited this location and time
        explored.push_back(top_node);
        // Populate frontier
        for (const auto& child : top_node.get_children(map_instance)) {
            // Check if child is constrained
            if (!is_constrained(constraints, agent, child, top_node.get_location())) {
                if (is_child_novel(frontier, explored, child)) {
                    frontier.push(child);
                } else if (auto costly_child_opt
                           = frontier.contains_more_expensive(child, child.get_f_value())) {
                    frontier.replace(*costly_child_opt, child);
                }
            }
        }
    }
    // No solution is found
    throw std::runtime_error("[multiastar] No solution  for agent " + std::to_string(agent));
}

bool is_child_novel(const Frontier& frontier,
                    const std::vector<Node>& explored,
                    const Node& child) {
    bool is_not_in_explored
        = std::find(explored.cbegin(), explored.cend(), child) == explored.cend();
    bool is_not_in_frontier = !frontier.contains(child);
    return is_not_in_explored && is_not_in_frontier;
}

bool can_path_end_here(int agent,
                       const Node& ending_node,
                       const ConstraintsContainer& constraints) {
    int timestep{ending_node.get_g_value()};
    std::vector<Constraint> my_constraints{constraints.greater_equal_timestep(timestep)};
    return std::none_of(my_constraints.cbegin(),
                        my_constraints.cend(),
                        [agent, &ending_node](const auto& constraint) -> bool {
                            return constraint.to_position == ending_node.get_location()
                                   && constraint.agent == agent;
                        });
}

bool is_constrained(const ConstraintsContainer& constraints,
                    int agent,
                    const Node& child,
                    const Point& from_position) {
    int timestep{child.get_g_value()};
    const auto& my_constraints{constraints.less_equal_timestep(timestep)};
    if (std::find_if(my_constraints.cbegin(),
                     my_constraints.cend(),
                     [&](const Constraint& constraint) -> bool {
                         return
                             // base check
                             (constraint.agent == agent && constraint.from_position == from_position
                              && constraint.to_position == child.get_location())
                             && (
                                 // normal constraint
                                 (constraint.timestep == child.get_g_value()) ||
                                 // final constraint
                                 (constraint.final && constraint.timestep <= child.get_g_value()));
                     })
        != my_constraints.cend()) {
        return true;
    }
    return false;
}

}  // namespace cmapd::multi_a_star