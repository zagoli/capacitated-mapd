//
// Created by Jacopo on 25/10/2022.
//

#include "Frontier.h"

namespace cmapd::multi_a_star {

Node Frontier::pop() {
    if (empty()) throw std::runtime_error("The frontier is empty.");
    Node best_node{
        *std::min_element(m_queue.cbegin(), m_queue.cend(), [](const Node& a, const Node& b) {
            return a.get_f_value() < b.get_f_value();
        })};
    m_queue.remove(best_node);
    return best_node;
}

void Frontier::replace(const Node& old_node, const Node& new_node) {
    if (empty()) throw std::runtime_error("The frontier is empty.");
    if (m_queue.remove(old_node) == 0) {
        throw std::runtime_error("old_node is not in the frontier.");
    }
    m_queue.push_front(new_node);
}

void Frontier::push(const Node& node) { m_queue.push_front(node); }

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

}  // namespace cmapd::multi_a_star