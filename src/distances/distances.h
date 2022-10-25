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

#include "AmbientMapInstance.h"
#include "Point.h"

namespace cmapd {
/** A type alias for the h-table, provided for ease of use.
 * h_table_t is a map which contains an entry for every non-wall cell 'A' of the instance.
 * Every entry contains a map with an entry for every starting and ending point 'B' of
 * every task. Every entry contains the distance between 'A' and 'B'.
 * For example, to get the distance between point (1,1) and (2,2) you should write
 * h_table.at({1,1}).at({2,2})
 */
using h_table_t = std::map<Point, std::map<Point, int>>;

/**
 * Computes the h_table for the provided map_instance with the provided distance_function.
 * @param map_instance The AmbientMapInstance for which the h_table is calculated.
 * @param distance_function This function should provide a distance between two Points.
 * @return The computed h_table.
 */
[[nodiscard]] h_table_t compute_h_table(const AmbientMapInstance& map_instance,
                                        auto distance_function);
/**
 * @brief Computes the manhattan distance between two Points.
 * @param first The first point.
 * @param second The second point.
 * @return The computed manhattan distance between first and second.
 */
int manhattan_distance(Point first, Point second);

namespace multi_a_star {
/**
 * This function is used by the A* algorithm to compute an estimated distance of a Point from a
 * sequence of goals.
 * @param location The point from which we need to calculate the distance
 * @param label The number of goal locations in goal_sequence that the current A* path has already
 * visited.
 * @param h_table The h-table (table of distances) for the desired map instance.
 * @param goal_sequence The goals the current A* path needs to visit.
 * @return The distance of location to the goals, according to label.
 * @see Lifelong Multi-Agent Path Finding in Large-Scale Warehouses, section 4.1
 */
int compute_h_value(Point location, int label, const h_table_t& h_table,
                    const std::vector<Point>& goal_sequence);
}  // namespace multi_a_star

}  // namespace cmapd