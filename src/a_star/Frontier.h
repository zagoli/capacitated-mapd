//
// Created by Jacopo on 25/10/2022.
//

#pragma once
#include <forward_list>
#include <optional>

#include "Node.h"

namespace cmapd::multi_a_star {

class Frontier {
  private:
    std::forward_list<Node> m_queue;

  public:
    void push(const Node& node);
    [[nodiscard]] Node pop();
    [[nodiscard]] bool contains_point(const Point& point) const;
    [[nodiscard]] std::optional<Node> contains_point_more_expensive(const Point& point,
                                                                    int cost) const;
    [[nodiscard]] bool empty() const;
    void replace(const Node& old_node, const Node& new_node);
};

}  // namespace cmapd::multi_a_star
