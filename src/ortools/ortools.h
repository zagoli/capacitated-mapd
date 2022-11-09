/**
 * @file ortools.h
 * @brief Contains the method assign_tasks.
 * @author Jacopo Zagoli
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#pragma once
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd {

/**
 * This function uses the OR-Tools library from Google to assign tasks to every agent.
 * @param instance The map instance on which the agents and tasks are.
 * @param capacity The maximum number of tasks an agent is able to carry.
 * @param h_table The h-table with the precomputed distances for the map.
 * @return A vector of goal sequences, one for every agent.
 */
std::vector<path_t> assign_tasks(const AmbientMapInstance& instance,
                                 int capacity,
                                 const h_table_t& h_table);
}  // namespace cmapd