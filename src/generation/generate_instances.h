/**
 * @file generate_instances.h
 * @brief Contains a function to generate ramdom instances of the map.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <ambient/AmbientMap.h>
#include <ambient/AmbientMapInstance.h>

#include <filesystem>
#include <vector>

namespace cmapd {
/**
 * Randomly generates a vector of AmbientMapInstance from a given AmbientMap
 * @param map AmbientMap reference on which generate instances
 * @param save_path path to a directory where to save all generated instances
 * @param n_instances number of instances to generate
 * @param n_agents number of agents on the map
 * @param n_tasks number of task on the map
 * @return a vector of AmbientMapInstance to solve
 */
std::vector<AmbientMapInstance> generate_instances(const AmbientMap& map,
                                                   const std::filesystem::path& save_path,
                                                   int n_instances,
                                                   int n_agents,
                                                   int n_tasks);
}
