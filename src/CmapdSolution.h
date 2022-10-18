/**
 * @file CmapdSolution.h
 * @brief Contains the struct CmapdSolution.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <vector>
#include "Point.h"

/**
 * @struct CmapdSolution
 * @brief Represents a solution to a CMAP instance.
 */
namespace cmapd {
struct CmapdSolution {
    /// A vector containing a path (vector of points) for every agent.
    const std::vector<std::vector<Point>> paths;
    /// The length of the longest path
    const std::size_t makespan;
    /// The sum of all paths lengths
    const std::size_t cost;
    /// Number of resolved conflicts
    const int conflicts;
};
}