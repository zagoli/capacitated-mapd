/**
 * @file distances.h
 * @brief Contains various functions related to distances between cells of the map.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <map>

#include "Point.h"

// TODO: replace with header import ---------------------
#include "../AmbientMap.h"
class AmbientMapInstance : public cmapd::AmbientMap {
  public:
    size_t get_num_agents();
    size_t get_num_tasks();
    std::vector<std::pair<cmapd::Point, cmapd::Point>> get_tasks() const;
};
// TODO: -------------------------------------------------

namespace cmapd {
/// A type alias for the h-table, provided for ease of use.
using h_table_t = std::map<Point, std::map<Point, int>>;

[[nodiscard]] h_table_t compute_h_table(const AmbientMapInstance& map_instance,
                                        auto distance_function);
/**
 * @brief Computes the manhattan distance between two Points.
 * @param[in] first The first point.
 * @param[in] second The second point.
 * @return The computed manhattan distance between first and second.
 */
int manhattan_distance(Point first, Point second);

namespace multi_a_star {
int compute_h_value(Point location, int label, const h_table_t& h_table,
                    const std::vector<Point>& goal_sequence);
}

}  // namespace cmapd