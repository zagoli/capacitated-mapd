//
// Created by Jacopo on 18/10/2022.
//

#pragma once
#include <vector>

#include "AmbientMap.h"
#include "Point.h"
#include "distances/distances.h"

namespace cmapd::multi_a_star {

class Node {
  private:
    const Point m_location;
    const Node* m_parent;
    int m_label;
    const int m_g;
    const int m_h;
    const h_table_t& m_h_table;
    const std::vector<Point>& m_goal_sequence;

  public:
    explicit Node(Point loc, const h_table_t& h_table, const std::vector<Point>& goal_sequence);
    explicit Node(Point loc, const Node& parent, const h_table_t& h_table,
                  const std::vector<Point>& goal_sequence);
    [[nodiscard]] std::vector<Node> get_children(const AmbientMapInstance& ambient_map) const;
    [[nodiscard]] std::vector<Point> get_path() const;
};

}  // namespace cmapd::multi_a_star