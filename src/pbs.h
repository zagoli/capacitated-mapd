/**
 * @file pbs.h
 * @brief Contains the pbs solver function.
 * @author Davide Furlani
 * @version 1.0
 * @date November, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#pragma once

#include "a_star/multi_a_star.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd {

/**
 * Calculate path for each agent given the sequence of goal assigned by the ortools library
 * @param instance the instance to solve
 * @param ot_paths the sequence of goals for each agent
 * @param h_table the distance matrix
 * @return the detailed path for each agent
 */
std::vector<path_t> pbs(const AmbientMapInstance& instance, std::vector<path_t> ot_paths) {
    std::vector<Constraint> constraints{};
    std::vector<path_t> paths{};

    for (int i = 0; static_cast<size_t>(i) < ot_paths.size();
         ++i) {  // per ogni sequenza di goal (una per agente) calcolo il percorso con multi A*
        path_t path = multi_a_star::multi_a_star(
            i, instance.get_agents().at(i), ot_paths.at(i), instance, constraints);
        paths.push_back(path);
        for (int t = 0; static_cast<size_t>(t) < path.size();
             ++t) {  // una volta calcolato il percorso aggiungo i constraint per ogni altro agente
            Point p{path.at(t)};
            for (int a = i + 1; a < instance.get_num_agents(); ++a) {
                constraints.emplace_back(Constraint{a, t, p, p});

                Point up{p.row - 1, p.col};
                if (instance.is_valid_position(up))
                    constraints.emplace_back(Constraint{a, t, up, p});

                Point down{p.row + 1, p.col};
                if (instance.is_valid_position(down))
                    constraints.emplace_back(Constraint{a, t, down, p});

                Point right{p.row, p.col + 1};
                if (instance.is_valid_position(right))
                    constraints.emplace_back(Constraint{a, t, right, p});

                Point left{p.row, p.col - 1};
                if (instance.is_valid_position(left))
                    constraints.emplace_back(Constraint{a, t, left, p});
            }
        };
    }
    return paths;
}

}  // namespace cmapd
