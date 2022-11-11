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
#include "custom_types.h"

/**
 * @struct CmapdSolution
 * @brief Represents a solution to a CMAPD instance.
 */
namespace cmapd {
struct CmapdSolution {
    /// A vector containing a path (vector of points) for every agent.
    const std::vector<path_t> paths;
    /// The length of the longest path
    const int makespan;
    /// The sum of all paths lengths
    const int cost;
};
}  // namespace cmapd