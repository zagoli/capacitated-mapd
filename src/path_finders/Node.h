/**
 * @file Node.h
 * @brief Contains the class Node used by cbs algorithm.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <optional>
#include <utility>
#include <vector>

#include "Conflict.h"
#include "Constraint.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::cbs {

/**
 * @class Node
 * @brief Represent a node of the cbs algorithm.
 */
class Node {
  private:
    /// the paths of the current node, one for every agent.
    std::vector<path_t> m_paths;
    /**
     * Detect the first conflict in the provided paths, if present.
     * @param first_agent The number of the first agent.
     * @param second_agent The number of the second agent.
     * @param first_path The path of the first agent.
     * @param second_path The path of with the second agent.
     * @return An optional containing the first conflict, if found, otherwise an empty optional.
     */
    static std::optional<Conflict> detect_conflict(int first_agent,
                                                   int second_agent,
                                                   const path_t& first_path,
                                                   const path_t& second_path);

  public:
    /**
     * Constructor for a Node.
     * @param instance The map instance on which we are operating.
     * @param goal_sequences The goal sequences for every agent.
     * @param constraints The constraints to take into account when computing paths.
     */
    explicit Node(const AmbientMapInstance& instance,
                  std::vector<path_t> goal_sequences,
                  const std::vector<Constraint>& constraints = {});
    /**
     * Get the lengths of every path.
     * @return a vector containing the length of every computed path.
     */
    [[nodiscard]] std::vector<int> get_lengths();
    /**
     * Get the maximum length of paths.
     * @return the maximum length of paths.
     */
    [[nodiscard]] int get_makespan();
    /**
     * Get the first conflict between every path, if found.
     * @return an optional containing the first conflict, if found, otherwise an empty optional.
     */
    [[nodiscard]] std::optional<Conflict> get_first_conflict();
    /**
     * Get the computed paths.
     * @return the computed paths, one for every agent.
     */
    [[nodiscard]] const std::vector<path_t>& get_paths() const;
};

}  // namespace cmapd::cbs
