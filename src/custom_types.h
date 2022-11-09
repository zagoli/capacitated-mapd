/**
 * @file custom_types.h
 * @brief Contains type aliases used by the project.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <map>
#include <vector>

#include "Point.h"

namespace cmapd {
/// A type alias for a path, provided for ease of use.
using path_t = std::vector<Point>;
/** A type alias for the h-table, provided for ease of use.
 * h_table_t is a map which contains an entry for every non-wall cell 'A' of the instance.
 * Every entry contains a map with an entry for every starting and ending point 'B' of
 * every task. Every entry contains the distance between 'A' and 'B'.
 * For example, to get the distance between point (1,1) and (2,2) you should write
 * h_table.at({1,1}).at({2,2})
 */
using h_table_t = std::map<Point, std::map<Point, int>>;

using moves_t = std::vector<std::pair<int, int>>;
}  // namespace cmapd
