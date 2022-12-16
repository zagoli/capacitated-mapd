/**
 * @file
 * @brief Contains the implementation of cbs methods.
 * @author Jacopo Zagoli
 * @version 1.1
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

#include "CmapdSolution.h"
#include "Conflict.h"
#include "ConflictType.h"
#include "Constraint.h"
#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"
#include "path_finders/Node.h"

namespace cmapd::cbs {

std::vector<Constraint> generate_constraints(const Conflict& conflict,
                                             int agent_num,
                                             const AmbientMapInstance& instance);

CmapdSolution cbs(const AmbientMapInstance& instance, const std::vector<path_t>& goal_sequences) {
    // Compare two cbs nodes based on the cost, and then on the number of conflicts.
    auto node_comparator = [](const Node& a, const Node& b) -> bool {
        if (a.cost() != b.cost()) {
            return a.cost() > b.cost();
        } else {
            return a.num_conflicts() > b.num_conflicts();
        }
    };
    // The frontier with all the nodes
    std::priority_queue<Node, std::vector<Node>, decltype(node_comparator)> frontier{
        node_comparator};

    // 1. create root node
    Node root{instance, goal_sequences};
    // 2. push root in frontier
    frontier.push(std::move(root));
    // 3. while frontier not empty
    while (!frontier.empty()) {
        // 4. pop node
        auto node = frontier.top();
        frontier.pop();
        // 5. get first conflict
        std::optional<Conflict> conflict{node.first_conflict()};
        // 6. if conflict not found, solution found
        if (!conflict) {
            return {.paths = node.get_paths(), .makespan = node.makespan(), .cost = node.cost()};
        }
        // 7. if conflict found, create two nodes: one with constraint for first agent, one
        //    with constraints for second agent

        // first node
        auto first_new_constraints = generate_constraints(conflict.value(), 1, instance);
        std::vector<Constraint> first_constraints{node.get_constraints()};
        first_constraints.insert(
            first_constraints.end(), first_new_constraints.begin(), first_new_constraints.end());
        Node first{instance,
                   goal_sequences.at(conflict->first_agent),
                   std::move(first_constraints),
                   node,
                   conflict->first_agent};

        // second node
        auto second_new_constraints = generate_constraints(conflict.value(), 2, instance);
        std::vector<Constraint> second_constraints{node.get_constraints()};
        second_constraints.insert(
            second_constraints.end(), second_new_constraints.begin(), second_new_constraints.end());
        Node second{instance,
                    goal_sequences.at(conflict->second_agent),
                    std::move(second_constraints),
                    node,
                    conflict->second_agent};

        // 8. push nodes in the queue
        frontier.push(first);
        frontier.push(second);
    }
    // 9. if frontier is empty, no solution is found
    throw std::runtime_error{"Cbs didn't find a solution."};
}

/**
 * Generate constraints for a Conflict.
 * @param conflict The conflict for which the constraints are generated.
 * @param agent_num The involved agent.
 * @param instance The AmbientMapInstance for which we generate constraints.
 * @return a vector of generated constraints.
 */
std::vector<Constraint> generate_constraints(const Conflict& conflict,
                                             int agent_num,
                                             const AmbientMapInstance& instance) {
    if (agent_num != 1 && agent_num != 2)
        throw std::invalid_argument{"Agent number must be 1 or 2."};
    std::vector<Constraint> constraints{};
    if (conflict.type == ConflictType::EDGE) {
        if (agent_num == 1) {
            constraints.emplace_back(Constraint{conflict.first_agent,
                                                conflict.timestep,
                                                conflict.first_position,
                                                conflict.second_position});
        } else {
            constraints.emplace_back(Constraint{conflict.second_agent,
                                                conflict.timestep,
                                                conflict.second_position,
                                                conflict.first_position});
        }
    } else if (conflict.type == ConflictType::VERTEX) {
        int agent;
        if (agent_num == 1) {
            agent = conflict.first_agent;
        } else {
            agent = conflict.second_agent;
        }
        for (moves_t moves{{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}}; const auto& move : moves) {
            Point from_where = conflict.first_position + move;
            if (instance.is_valid(from_where)) {
                constraints.emplace_back(
                    Constraint{agent, conflict.timestep, from_where, conflict.second_position});
            }
        }
    }
    return constraints;
}

}  // namespace cmapd::cbs
