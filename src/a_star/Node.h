/**
 * @file
 * @brief Contains the class Node in namespace multi_a_star.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <compare>
#include <vector>

#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::multi_a_star {

/**
 * @class Node
 * @brief This class represents a Node that is used in the multi A* search algorithm.
 * @see Lifelong Multi-Agent Path Finding in Large-Scale Warehouses.
 */
class Node {
    friend class std::hash<cmapd::multi_a_star::Node>;

  private:
    /// A location on the map.
    Point m_location;
    /// The path from root to the current node. For root is root itself.
    path_t m_path;
    /// The number of goal locations in goal_sequence that the current A* path has already visited.
    int m_label;
    /// The cost of the path until the current Node.
    int m_g;
    /// The estimated cost of visiting the goals from the current Node.
    int m_h;
    /// A reference to the h-table, useful for generating other nodes.
    h_table_t m_h_table;
    /// Goal to visit.
    path_t m_goal_sequence;

  public:
    /**
     * Constructor for the root Node.
     * @param loc Position on the map.
     * @param h_table A reference to the h-table for the current map.
     * @param goal_sequence The goals to visit.
     */
    explicit Node(Point loc, const h_table_t& h_table, const path_t& goal_sequence);
    /**
     * Constructor for a Node with a parent.
     * @param loc Position on the map.
     * @param parent A reference to the parent Node.
     * @param h_table A reference to the h-table for the current map.
     * @param goal_sequence The goals to visit.
     */
    explicit Node(Point loc,
                  const Node& parent,
                  const h_table_t& h_table,
                  const path_t& goal_sequence);
    /**
     * This method returns all children in valid positions of a Node with all the parameters set.
     * @param instance The map used to find the path.
     * @return A vector of child Nodes.
     */
    [[nodiscard]] std::vector<Node> get_children(const AmbientMapInstance& instance) const;
    /**
     * This method returns the positions from the parent to the current Node following the path.
     * The parent and the current node are included.
     * @return A vector of Points.
     */
    [[nodiscard]] path_t get_path() const;
    /// Location getter.
    [[nodiscard]] const Point& get_location() const;
    /// Label getter.
    [[nodiscard]] int get_label() const;
    /// Increment label value by one.
    void increment_label();
    /// Equality operator.
    bool operator==(const Node& rhs) const;
    /// Decrement label value by one.
    void decrement_label();
    /// Get the f-value of the node, that is, g-value + h-value;
    [[nodiscard]] int get_f_value() const;
    /// Get the g-value of the node.
    [[nodiscard]] int get_g_value() const;
};

}  // namespace cmapd::multi_a_star