/**
 * @file
 * @brief Contains the class Node used by cbs algorithm.
 * @author Jacopo Zagoli
 * @version 1.1
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <optional>
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
    /// the constraints of the current node
    std::vector<Constraint> m_constraints;
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
     * @throws runtime_error if multi_a_star can't find a path for one agent.
     */
    explicit Node(const AmbientMapInstance& instance,
                  std::vector<path_t> goal_sequences,
                  std::vector<Constraint>&& constraints = {});
    /**
     * Constructor for a child Node.
     * @param node The parent Node.
     * @param agent The agent for which we need to compute the path again.
     * @param constraints The constraints to take into account when computing paths.
     * @param goal_sequence The goal sequence for agent.
     * @param instance The map instance on which we are operating.
     * @throws runtime_error if multi_a_star can't find a path for one agent.
     */
    explicit Node(const Node& node,
                  int agent,
                  std::vector<Constraint>&& constraints,
                  path_t goal_sequence,
                  const AmbientMapInstance& instance);

    /**
     * Get the lengths of every path.
     * @return a vector containing the length of every computed path.
     */
    [[nodiscard]] std::vector<int> lengths() const;
    /**
     * Get the maximum length of paths.
     * @return the maximum length of paths.
     */
    [[nodiscard]] int makespan() const;
    /**
     * Get the sum of all the paths lengths.
     * @return the sum of all the paths lengths.
     */
    [[nodiscard]] int cost() const;
    /**
     * Get the first conflict between every path, if found.
     * @return an optional containing the first conflict, if found, otherwise an empty optional.
     */
    [[nodiscard]] std::optional<Conflict> first_conflict() const;
    /**
     * Get the number of conflicts in the calculated paths.
     * @return the number of conflicts in the calculated paths.
     */
    [[nodiscard]] int num_conflicts() const;
    /**
     * Get the computed paths.
     * @return the computed paths, one for every agent.
     */
    [[nodiscard]] std::vector<path_t> get_paths() const;
    /**
     * Get the constraints of the current node.
     * @return the constraints of the current node.
     */
    [[nodiscard]] std::vector<Constraint> get_constraints() const;
};

}  // namespace cmapd::cbs
