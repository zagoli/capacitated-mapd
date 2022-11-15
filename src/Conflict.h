/**
 * @file
 * @brief Contains the struct conflict.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include "ConflictType.h"
#include "Point.h"

namespace cmapd {

/**
 * @struct Conflict
 * @brief Represents a conflict between agents when following a path.
 */
struct Conflict {
    /// The first agent in the conflict.
    int first_agent;
    /// The second agent in the conflict.
    int second_agent;
    /// The moment when the conflict occurs.
    int timestep;
    /// For the vertex conflict: the position where the conflict occurs.
    /// For the edge conflict: the position where the first agent is moving from.
    Point first_position;
    /// For the vertex conflict: equal to first_position.
    /// For the edge conflict: the position where the first agent is moving to.
    Point second_position;
    /// Type of the conflict.
    ConflictType type;
    /// Defines equality between conflicts comparing all fields.
    bool operator==(const Conflict& rhs) const = default;
};

}  // namespace cmapd
