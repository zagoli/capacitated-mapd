//
// Created by dade on 10/11/22.
//
#include "pbs.h"

#include "Constraint.h"
#include "a_star/multi_a_star.h"

namespace cmapd::pbs {

CmapdSolution pbs(AmbientMapInstance instance, const std::vector<path_t>& goal_sequences) {
    std::vector<Constraint> constraints{};
    std::vector<path_t> paths{};

    for (int i = 0; static_cast<size_t>(i) < goal_sequences.size();
         ++i) {  // per ogni sequenza di goal (una per agente) calcolo il percorso con multi A*
        path_t path = multi_a_star::multi_a_star(
            i, instance.agents().at(i), goal_sequences.at(i), instance, constraints);
        paths.push_back(path);
        for (int t = 0; static_cast<size_t>(t) < path.size();
             ++t) {  // una volta calcolato il percorso aggiungo i constraint per ogni altro agente
            Point p{path.at(t)};
            for (int a = i + 1; a < instance.num_agents(); ++a) {
                constraints.emplace_back(Constraint{a, t, p, p});

                Point up{p.row - 1, p.col};
                if (instance.is_valid(up)) constraints.emplace_back(Constraint{a, t, up, p});

                Point down{p.row + 1, p.col};
                if (instance.is_valid(down)) constraints.emplace_back(Constraint{a, t, down, p});

                Point right{p.row, p.col + 1};
                if (instance.is_valid(right)) constraints.emplace_back(Constraint{a, t, right, p});

                Point left{p.row, p.col - 1};
                if (instance.is_valid(left)) constraints.emplace_back(Constraint{a, t, left, p});
            }
            instance.wall(path.back());
        }
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