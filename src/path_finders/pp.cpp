/**
 * @file
 * @brief Contains the pp method implementation.
 * @author Davide Furlani
 * @author Jacopo Zagoli
 * @version 1.2
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include <vector>

#include "CmapdSolution.h"
#include "ConstraintsContainer.h"
#include "Point.h"
#include "a_star/multi_a_star.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::pp {

/// Generates the constraints for a particular path.
ConstraintsContainer& add_constraints_from_path(int agent,
                                                const path_t& path,
                                                const AmbientMapInstance& instance,
                                                ConstraintsContainer& constraints);

/// Compute the cost and makespan and return them in a CmapdSolution with the paths.
CmapdSolution compute_solution(std::vector<path_t>& paths);

CmapdSolution pp(const AmbientMapInstance& instance, std::vector<path_t> goal_sequences) {
    ConstraintsContainer constraints{};
    std::vector<path_t> paths{};

    for (int agent = 0; agent < goal_sequences.size(); ++agent) {
        // Computing path
        auto& goal_sequence{goal_sequences.at(agent)};
        auto start_location{goal_sequence.at(0)};
        // remove start location from goal_sequence if it's not the only one
        if (goal_sequence.size() != 1) {
            goal_sequence.erase(goal_sequence.begin());
        }
        path_t path = multi_a_star::multi_a_star(
            agent, start_location, goal_sequence, instance, constraints);
        paths.push_back(path);
        constraints = add_constraints_from_path(agent, path, instance, constraints);
    }
    return compute_solution(paths);
}

CmapdSolution compute_solution(std::vector<path_t>& paths) {
    int makespan{};
    int cost{};
    for (const path_t& p : paths) {
        const int path_size = static_cast<int>(p.size());
        makespan = std::max(path_size, makespan);
        cost += path_size;
    }
    return {paths, makespan, cost};
}

ConstraintsContainer& add_constraints_from_path(int agent,
                                                const path_t& path,
                                                const AmbientMapInstance& instance,
                                                ConstraintsContainer& constraints) {
    for (int timestep = 0; timestep < path.size(); ++timestep) {
        Point point{path.at(timestep)};
        // Vertex
        for (moves_t moves{{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}}; const auto& move : moves) {
            Point from_where{point + move};
            if (instance.is_valid(from_where)) {
                // if this is the last timestep, final should equal to true
                constraints.add_constraint(
                    {agent, timestep, from_where, point, timestep == path.size() - 1});
            }
        }
        // Edge
        if (timestep < path.size() - 1) {
            constraints.add_constraint({agent, timestep + 1, path.at(timestep + 1), point});
        }
    }
    return constraints;
}

}  // namespace cmapd::pp
