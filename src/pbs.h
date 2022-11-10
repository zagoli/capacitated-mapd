//
// Created by dade on 08/11/22.
//

#pragma once

#include "a_star/multi_a_star.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd {

std::vector<path_t> pbs(AmbientMapInstance instance,
                        std::vector<path_t> ot_paths,
                        h_table_t h_table) {
    std::vector<Constraint> constraints{};
    std::vector<path_t> paths{};

    for (int i = 0; static_cast<size_t>(i) < ot_paths.size(); ++i) {
        path_t path = multi_a_star::multi_a_star(
            i, instance.get_agents().at(i), ot_paths.at(i), instance, constraints, h_table);
        paths.push_back(path);
        for (int t = 0; static_cast<size_t>(t) < path.size(); ++t) {
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
