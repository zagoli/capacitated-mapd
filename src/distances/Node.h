/**
 * @file
 * @brief Contains the class Node for the BFS.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli
 */

#pragma once
#include "Point.h"

namespace cmapd::bfs {

/**
 * @struct Node
 * @brief a BFS node.
 */
struct Node {
    /// The point which we are exploring
    Point point;
    /// The cost of the path until this node.
    int path_cost = 0;
    /// default equality operator.
    inline bool operator==(const Node&) const = default;
};

}  // namespace cmapd::bfs
