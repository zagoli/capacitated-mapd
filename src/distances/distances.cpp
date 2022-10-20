//
// Created by Jacopo on 18/10/2022.
//
#include "distances.h"

namespace cmapd {

h_table_t compute_h_table(const AmbientMapInstance& map_instance, auto distance_function) {
    h_table_t hTable;
    // iterate over every cell of the grid
    for (int r{0}; r < map_instance.get_rows_number(); ++r) {
        for (int c{0}; c < map_instance.get_columns_number(); ++c) {
            Point current_cell{r, c};
            // iterate over every task to compute the distance
            for (const auto& [start, goal] : map_instance.get_tasks()) {
                // compute distance from current cell to the task starting point
                hTable[current_cell][start] = distance_function(current_cell, start);
                // compute distance from current cell to the task ending point
                hTable[current_cell][goal] = distance_function(current_cell, goal);
            }
        }
    }
    return hTable;
}

int manhattan_distance(const Point first, const Point second) {
    return abs(first.row - second.row) + abs(first.col - second.col);
}

namespace multi_a_star {
int compute_h_value(const Point x, const int label, const h_table_t& h_table,
                    const std::vector<Point>& goal_sequence) {
    int h_value{h_table.at(x).at(goal_sequence[label])};
    for (int j{label + 1}; j < goal_sequence.size(); ++j) {
        h_value += h_table.at(goal_sequence[j - 1]).at(goal_sequence[j]);
    }
    return h_value;
}
}  // namespace multi_a_star

}  // namespace cmapd