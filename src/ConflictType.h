/**
 * @file
 * @brief Contains the ConflictType enum.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once

namespace cmapd {

/**
 * @enum ConflictType
 * @brief Describes the type of a conflict.
 */
enum class ConflictType {
    /// When two agents wants to be in the same position at the same time.
    EDGE,
    /// When two agents switch positions.
    /// Example: at the same moment, agent 1 goes from A to B, and agent 2 goes from B to A.
    VERTEX
};

}  // namespace cmapd
