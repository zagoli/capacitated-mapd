/**
 * @file Frontier.h
 * @brief Contains the class Frontier.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <forward_list>
#include <optional>

#include "Node.h"

namespace cmapd::multi_a_star {

/**
 * @class Frontier
 * @brief This class is a sort of priority queue, which also allows to replace elements in it
 * and to test if an object is contained in the queue. For now it contains multi A* nodes.
 */
class Frontier {
  private:
    /// The underlying data structure containing the Nodes.
    std::forward_list<Node> m_queue;

  public:
    /**
     * Insert a Node in the frontier.
     * @param node The Node to be inserted.
     */
    void push(const Node& node);
    /**
     * Retrieve the Node with the minimum f-value in the frontier and remove it from the frontier.
     * @return The Node with the minimum f-value in the frontier.
     * @throws runtime_error if the frontier is empty.
     */
    [[nodiscard]] Node pop();
    /**
     * Test if a Node is present inside the frontier.
     * @param node The Node to be searched in the frontier.
     * @return True if the Node is present, false otherwise.
     */
    [[nodiscard]] bool contains(const Node& node) const;
    /**
     * Test if a Node is present inside the frontier, with a f-value greater than
     * the specified cost.
     * @param node The Node to be searched in the frontier.
     * @param cost The f-value to compare with Nodes in the frontier.
     * @return An optional containing a Node if found and nothing otherwise.
     */
    [[nodiscard]] std::optional<Node> contains_more_expensive(const Node& node,
                                                                    int cost) const;
    /**
     * Test if the frontier is empty.
     * @return True if the frontier is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const;
    /**
     * Replace a given Node in the frontier with another one.
     * @param old_node The Node to be replaced in the frontier.
     * @param new_node The Node which will replace the old one.
     * @throws runtime_error if the frontier is empty.
     * @throws runtime_error if old_node is not in the frontier.
     */
    void replace(const Node& old_node, const Node& new_node);
};

}  // namespace cmapd::multi_a_star
