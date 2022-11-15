/**
 * @file
 * @brief Contains the cbs method.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once

#include <vector>

#include "CmapdSolution.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::cbs {

/**
 * This function finds paths without conflicts for every agent using a Conflict Based Search.
 * @param instance The ambient map instance on which we are operating.
 * @param goal_sequences A vector containing a goal sequence for every agent.
 * @return a solution, if found.
 * @throws runtime_error if no solution is found.
 * @see Conflict-Based Search For Optimal Multi-Agent Path Finding.
 */
CmapdSolution cbs(const AmbientMapInstance& instance, const std::vector<path_t>& goal_sequences);

}
