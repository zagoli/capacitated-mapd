/**
 * @file
 * @brief declaration of multi_a_star function.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include "Constraint.h"
#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::multi_a_star {

/**
 * Computes the shortest path from the start_location to all goals specified in goal_sequence,
 * respecting their order in the vector. It takes into account the m_constraints in vector
 * m_constraints.
 * @param agent The integer representing the agent for which we are computing the path.
 * @param start_location The start location of the agent.
 * @param goal_sequence The sequence of goals to be visited.
 * @param map_instance The AmbientMapInstance on which the agents are moving.
 * @param constraints A vector of m_constraints to be respected when computing the path.
 * @return A vector of Point representing the found path.
 * @throws runtime_error if no path is found.
 * @see Lifelong Multi-Agent Path Finding in Large-Scale Warehouses.
 * @see Artificial Intelligence A Modern Approach, third edition, chapter 3, section 5.2
 */
path_t multi_a_star(int agent,
                    Point start_location,
                    const path_t& goal_sequence,
                    const AmbientMapInstance& map_instance,
                    const std::vector<Constraint>& constraints);

}  // namespace cmapd::multi_a_star