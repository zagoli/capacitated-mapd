/**
 * @file
 * @brief Contains the class ConstraintContainer.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date January, 2023
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <unordered_map>
#include <vector>

#include "Constraint.h"

namespace cmapd {
/**
 * @class ConstraintsContainer
 * @brief Container for constraints, organized them by timestep.
 */
class ConstraintsContainer {
  private:
    /// The underlying data structure.
    std::unordered_map<int, std::vector<Constraint>> m_constraints_map;

  public:
    /**
     * Add a single constraint.
     * @param constraint
     */
    void add_constraint(const Constraint& constraint);
    /**
     * Add a vector of constraints.
     * @param constraints
     */
    void add_constraints(const std::vector<Constraint>& constraints);
    /**
     * Get the constraints with a specific timestep.
     * @param timestep
     * @return a vector of constraints.
     */
    std::vector<Constraint> at_timestep(int timestep) const;
    /**
     * Get the constraints with timestep equal or greater than specified.
     * @param timestep
     * @return a vector of constraints.
     */
    std::vector<Constraint> greater_equal_timestep(int timestep) const;
};

}  // namespace cmapd
