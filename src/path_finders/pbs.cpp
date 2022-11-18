/**
 * @file
 * @brief Contains the pbs method implementation.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include <vector>

#include "CmapdSolution.h"
#include "Constraint.h"
#include "Point.h"
#include "a_star/multi_a_star.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::pbs {

CmapdSolution pbs(AmbientMapInstance instance, const std::vector<path_t>& goal_sequences) {
    std::vector<Constraint> constraints{};
    std::vector<path_t> paths{};

    for (int i = 0; static_cast<size_t>(i) < goal_sequences.size(); ++i) {
        path_t path = multi_a_star::multi_a_star(
            i, instance.agents().at(i), goal_sequences.at(i), instance, constraints);
        paths.push_back(path);
        for (int t = 0; static_cast<size_t>(t) < path.size(); ++t) {
            Point p{path.at(t)};
            for (int a = i + 1; a < instance.num_agents(); ++a) {
                for (moves_t moves{{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
                     const auto& move : moves) {
                    Point from_where = p + move;
                    if (instance.is_valid(from_where)) {
                        constraints.emplace_back(Constraint{a, t, from_where, p});
                    }
                }
            }
        }
        instance.wall(path.back());
    }
    int makespan{0};
    int cost{0};
    for (const path_t& p : paths) {
        if (static_cast<int>(p.size()) > makespan) makespan = static_cast<int>(p.size());
        cost += static_cast<int>(p.size());
    }

    return {paths, makespan, cost};
}
}  // namespace cmapd::pbs
