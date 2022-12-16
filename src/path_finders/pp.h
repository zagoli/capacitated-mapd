/**
 * @file
 * @brief Contains the pbs solver function.
 * @author Davide Furlani
 * @version 1.0
 * @date November, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#pragma once
#include <vector>

#include "CmapdSolution.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::pp {

/**
 * This function finds paths without conflicts for every agent using a Priority Based Search.
 * @param instance The ambient map instance on which we are operating.
 * @param goal_sequences A vector containing a goal sequence for every agent.
 * @return a solution, if found.
 * @throws runtime_error if no solution is found.
 */
CmapdSolution pp(const AmbientMapInstance& instance, std::vector<path_t> goal_sequences);
}  // namespace cmapd::pp
