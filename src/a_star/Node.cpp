//
// Created by Jacopo on 18/10/2022.
//

#include "Node.h"

#include "distances/distances.h"

namespace cmapd::multi_a_star {

Node::Node(const Point loc, const h_table_t& h_table, const std::vector<Point>& goal_sequence)
    : m_location{loc},
      m_parent{nullptr},
      m_label{0},
      m_g{0},
      m_h{cmapd::multi_a_star::compute_h_value(loc, m_label, h_table, goal_sequence)},
      m_h_table{h_table},
      m_goal_sequence{goal_sequence} {}

Node::Node(const Point loc, const Node& parent, const h_table_t& h_table,
           const std::vector<Point>& goal_sequence)
    : m_location{loc},
      m_parent{&parent},
      m_label{0},
      m_g{parent.m_g + 1},
      m_h{cmapd::multi_a_star::compute_h_value(loc, m_label, h_table, goal_sequence)},
      m_h_table{h_table},
      m_goal_sequence{goal_sequence} {}

std::vector<Node> Node::get_children(const AmbientMap& ambient_map) const {
    using moves_t = std::vector<std::pair<int, int>>;
    std::vector<Node> children;
    for (moves_t moves{{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}}; const auto& move : moves) {
        if (ambient_map.is_valid_position(m_location + move)) {
            children.emplace_back(m_location + move, *this, m_h_table, m_goal_sequence);
        }
    }
    return children;
}

std::vector<Point> Node::get_path() const {
    std::vector<Point> path;
    const Node* current = this;
    while (current != nullptr) {
        path.insert(path.cbegin(), current->m_location);
        current = current->m_parent;
    }
    return path;
}

}  // namespace cmapd::multi_a_star