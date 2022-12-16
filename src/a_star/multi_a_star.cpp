/**
 * @file
 * @brief Contains the implementation of multi_a_star.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "a_star/multi_a_star.h"

#include <set>

#include "Constraint.h"
#include "Point.h"
#include "a_star/Frontier.h"
#include "a_star/Node.h"
#include "custom_types.h"

namespace cmapd::multi_a_star {

/**
 * Check if an A* Node is present in the constraint list.
 * @param constraints The list of constraints.
 * @param agent The agent which we are checking.
 * @param child The node to be checked.
 * @param parent The parent of child.
 * @return true if child is constrained.
 */
bool is_constrained(const std::vector<Constraint>& constraints,
                    int agent,
                    const Node& child,
                    const Node& parent) {
    // create the constraint to be checked
    Constraint check_me{.agent = agent,
                        .timestep = child.get_g_value(),
                        .from_position = parent.get_location(),
                        .to_position = child.get_location()};
    if (std::find(constraints.cbegin(), constraints.cend(), check_me) != constraints.cend()) {
        return true;
    }
    // check for a previous final constraint
    if (std::find_if(constraints.cbegin(),
                     constraints.cend(),
                     [&check_me](const Constraint& constraint) -> bool {
                         return constraint.final && check_me.agent == constraint.agent
                                && constraint.timestep <= check_me.timestep
                                && check_me.from_position == constraint.from_position
                                && check_me.to_position == constraint.to_position;
                     })
        != constraints.cend()) {
        return true;
    }
    return false;
}

/**
 * Checks if a Node can be the last one of his path.
 * @param agent The agent we are computing the path for.
 * @param ending_node The candidate last Node.
 * @param constraints The constraint list.
 * @return true if there are no constraints on the location in ending_node after the current time.
 */
bool can_path_end_here(int agent,
                       const Node& ending_node,
                       const std::vector<Constraint>& constraints) {
    return std::none_of(constraints.cbegin(),
                        constraints.cend(),
                        [agent, &ending_node](const auto& constraint) -> bool {
                            return constraint.to_position == ending_node.get_location()
                                   && constraint.agent == agent
                                   && constraint.timestep >= ending_node.get_g_value();
                        });
}

path_t multi_a_star(int agent,
                    Point start_location,
                    const path_t& goal_sequence,
                    const AmbientMapInstance& map_instance,
                    const std::vector<Constraint>& constraints,
                    int timeout) {
    // compute timeout value
    if (timeout <= 0) {
        timeout = map_instance.rows_number() * map_instance.columns_number() * 10;
    }
    if (goal_sequence.empty()) {
        throw std::runtime_error{"[multiastar] the goal sequence is empty!"};
    }
    // frontier definition
    Frontier frontier;
    // explore set definition
    std::set<Node> explored;
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
        explored.insert(top_node);
        // Populate frontier
        for (const auto& child : top_node.get_children(map_instance)) {
            // Check if child is constrained
            if (!is_constrained(constraints, agent, child, top_node)) {
                if (!explored.contains(child) && !frontier.contains(child)) {
                    frontier.push(child);
                } else if (auto costly_child_opt
                           = frontier.contains_more_expensive(child, child.get_f_value())) {
                    frontier.replace(costly_child_opt.value(), child);
                }
            }
        }
    }
    // No solution is found
    throw std::runtime_error("[multiastar] No solution  for agent " + std::to_string(agent));
}

}  // namespace cmapd::multi_a_star