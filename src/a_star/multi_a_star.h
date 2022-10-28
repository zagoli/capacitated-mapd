//
// Created by Jacopo on 25/10/2022.
//

#pragma once
#include "Constraint.h"
#include "Node.h"

namespace cmapd::multi_a_star {

std::vector<Point> multi_a_star(int agent,
                                Point start_location,
                                const std::vector<Point>& goal_sequence,
                                const AmbientMapInstance& map_instance,
                                const std::vector<Constraint>& constraints,
                                const h_table_t& h_table);

}