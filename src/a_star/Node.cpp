/**
 * @file
 * @brief Contains the implementation of the multi A* Node.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "a_star/Node.h"

#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"
#include "distances/distances.h"

namespace cmapd::multi_a_star {

Node::Node(const Point loc, const h_table_t& h_table, const path_t& goal_sequence)
    : m_location{loc},
      m_path{{loc}},
      m_label{0},
      m_g{0},
      m_h{cmapd::multi_a_star::compute_h_value(loc, m_label, h_table, goal_sequence)},
      m_h_table{h_table},
      m_goal_sequence{goal_sequence} {}

Node::Node(const Point loc,
           const Node& parent,
           const h_table_t& h_table,
           const path_t& goal_sequence)
    : m_location{loc},
      m_path{parent.get_path()},
      m_label{parent.m_label},
      m_g{parent.m_g + 1},
      m_h{cmapd::multi_a_star::compute_h_value(loc, parent.m_label, h_table, goal_sequence)},
      m_h_table{h_table},
      m_goal_sequence{goal_sequence} {
    m_path.push_back(m_location);
}

std::vector<Node> Node::get_children(const AmbientMapInstance& instance) const {
    std::vector<Node> children;
    for (moves_t moves{{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}}; const auto& move : moves) {
        Point new_position = m_location + move;
        if (instance.is_valid(new_position)) {
            children.emplace_back(new_position, *this, m_h_table, m_goal_sequence);
        }
    }
    return children;
}

path_t Node::get_path() const { return m_path; }

std::partial_ordering Node::operator<=>(const Node& rhs) const {
    if (auto c = get_location() <=> rhs.get_location(); c != nullptr) return c;
    return m_g <=> rhs.m_g;
}

bool Node::operator==(const Node& rhs) const {
    return m_location == rhs.m_location && m_g == rhs.m_g;
}

const Point& Node::get_location() const { return m_location; }

int Node::get_label() const { return m_label; }

void Node::increment_label() { m_label++; }

int Node::get_f_value() const { return m_g + m_h; }

int Node::get_g_value() const { return m_g; }

void Node::decrement_label() { m_label--; }

}  // namespace cmapd::multi_a_star