//
// Created by Jacopo on 02/11/2022.
//

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
