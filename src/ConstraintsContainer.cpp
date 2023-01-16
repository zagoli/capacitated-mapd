//
// Created by Jacopo on 14/01/2023.
//

#include "ConstraintsContainer.h"

#include <vector>
namespace cmapd {

void ConstraintsContainer::add_constraint(const Constraint& constraint) {
    int timestep{constraint.timestep};
    auto& constraints{m_constraints_map[timestep]};
    constraints.push_back(constraint);
}

std::vector<Constraint> ConstraintsContainer::at_timestep(int timestep) const {
    if (m_constraints_map.contains(timestep)) {
        return m_constraints_map.at(timestep);
    } else {
        return {};
    }
}

std::vector<Constraint> ConstraintsContainer::greater_equal_timestep(int timestep) const {
    std::vector<Constraint> result;
    for (const auto& element : m_constraints_map) {
        if (element.first >= timestep) {
            result.insert(result.end(), element.second.begin(), element.second.end());
        }
    }
    return result;
}

std::vector<Constraint> ConstraintsContainer::less_equal_timestep(int timestep) const {
    std::vector<Constraint> result;
    for (const auto& element : m_constraints_map) {
        if (element.first <= timestep) {
            result.insert(result.end(), element.second.begin(), element.second.end());
        }
    }
    return result;
}

void ConstraintsContainer::add_constraints(const std::vector<Constraint>& constraints) {
    for (const auto& constraint : constraints) {
        add_constraint(constraint);
    }
}
}  // namespace cmapd