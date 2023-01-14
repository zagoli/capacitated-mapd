/**
 * @file
 * @brief Contains the implementation of the class Frontier.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#include "a_star/Frontier.h"

#include <optional>
#include <stdexcept>

#include "a_star/Node.h"

namespace cmapd::multi_a_star {

Node Frontier::pop() {
    if (empty()) throw std::runtime_error("The frontier is empty.");
    auto best_node_pos{
        std::min_element(m_queue.cbegin(), m_queue.cend(), [](const Node& a, const Node& b) {
            return a.get_f_value() <= b.get_f_value();
        })};
    Node result{*best_node_pos};
    m_queue.erase(best_node_pos);
    return result;
}

void Frontier::replace(const Node& old_node, const Node& new_node) {
    if (empty()) throw std::runtime_error("The frontier is empty.");
    auto old_node_position{std::find(m_queue.begin(), m_queue.end(), old_node)};
    if (old_node_position == m_queue.end()) {
        throw std::runtime_error("old_node is not in the frontier.");
    } else {
        m_queue.erase(old_node_position);
    }
    push(new_node);
}

void Frontier::push(const Node& node) { m_queue.push_back(node); }

bool Frontier::contains(const Node& node) const {
    return std::find(m_queue.cbegin(), m_queue.cend(), node) != m_queue.cend();
}

std::optional<Node> Frontier::contains_more_expensive(const Node& node, int cost) const {
    auto iter = std::find_if(m_queue.cbegin(), m_queue.cend(), [node, cost](const Node& n_iter) {
        return n_iter == node && n_iter.get_f_value() > cost;
    });
    if (iter == m_queue.cend()) return {};
    return {*iter};
}

bool Frontier::empty() const { return m_queue.empty(); }

Frontier::Frontier() = default;

Frontier::Frontier(int space_to_reserve) { m_queue.reserve(space_to_reserve); }

}  // namespace cmapd::multi_a_star