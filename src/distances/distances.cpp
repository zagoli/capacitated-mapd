//
// Created by Jacopo on 18/10/2022.
//
#include "distances.h"

namespace cmapd {

[[nodiscard]] hTable_t compute_HTable(const AmbientMapInstance& map_instance,
                                      auto distance_function) {
    hTable_t hTable;
    // iterate over every cell of the grid
    for (int r = 0; r < map_instance.get_rows(); ++r) {
        for (int c = 0; c < map_instance.get_columns(); ++c) {
            Point current_cell{r, c};
            // iterate over every task to compute the distance
            for (const auto task : map_instance.get_tasks()) {
                // compute distance from current cell to the task starting point
                hTable[current_cell][task.first] = distance_function(current_cell, task.first);
                // compute distance from current cell to the task ending point
                hTable[current_cell][task.second] = distance_function(current_cell, task.second);
            }
        }
    }
    return hTable;
}

unsigned manhattan_distance(const Point first, const Point second) {
    return abs(first.row - second.row) + abs(first.col - second.col);
}

}  // namespace cmapd