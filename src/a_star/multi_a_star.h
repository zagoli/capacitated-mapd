/**
 * @file Node.h
 * @brief declaration of multi_a_star function.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include "Constraint.h"
#include "Node.h"

namespace cmapd::multi_a_star {

/**
 * Computes the shortest path from the start_location to all goals specified in goal_sequence,
 * respecting their order in the vector. It takes into account the constraints in vector
 * constraints.
 * @param agent The integer representing the agent for which we are computing the path.
 * @param start_location The start location of the agent.
 * @param goal_sequence The sequence of goals to be visited.
 * @param map_instance The AmbientMapInstance on which the agents are moving.
 * @param constraints A vector of constraints to be respected when computing the path.
 * @param h_table Distance table used as heuristic when computing the path.
 * @return A vector of Point representing the found path.
 * @throws runtime_error if no path is found.
 */
std::vector<Point> multi_a_star(int agent,
                                Point start_location,
                                const std::vector<Point>& goal_sequence,
                                const AmbientMapInstance& map_instance,
                                const std::vector<Constraint>& constraints,
                                const h_table_t& h_table);

}  // namespace cmapd::multi_a_star