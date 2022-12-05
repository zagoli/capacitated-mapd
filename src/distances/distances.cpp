/**
 * @file
 * @brief Contains the implementation of distances methods.
 * @author Jacopo Zagoli
 * @version 2.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "distances/distances.h"

#include <algorithm>
#include <list>
#include <set>

#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"
#include "distances/Node.h"

namespace cmapd {

h_table_t compute_h_table(const AmbientMapInstance& map_instance) {
    h_table_t h_table;
    // Creation of a list with all points of interests (agents and tasks)
    std::vector<Point> poi{map_instance.agents()};
    for (auto& [start, goal] : map_instance.tasks()) {
        poi.push_back(start);
        poi.push_back(goal);
    }
    // We do a BFS starting from every poi to every other location on the map
    // iterate over every poi
    using namespace bfs;
    for (Point destination : poi) {
        // BFS algorithm
        Node root{destination};
        std::list<Node> frontier;
        std::set<Point> explored;
        frontier.push_back(root);
        while (!frontier.empty()) {
            // get first node and remove it
            Node node{frontier.front()};
            frontier.pop_front();
            // update h-table
            h_table[node.point][destination] = node.path_cost;
            // mark point as explored
            explored.insert(node.point);
            // generate child nodes
            for (moves_t moves{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; const auto& move : moves) {
                Node child{node.point + move, node.path_cost + 1};
                // check if child is valid or is in explored or frontier
                if (map_instance.is_valid(child.point) && !explored.contains(child.point)
                    && std::find(frontier.cbegin(), frontier.cend(), child) == frontier.cend()) {
                    frontier.push_back(child);
                }
            }
        }
    }
    return h_table;
}

namespace multi_a_star {

int compute_h_value(Point x, int label, const h_table_t& h_table, const path_t& goal_sequence) {
    int h_value{h_table.at(x).at(goal_sequence[label])};
    for (int j{label + 1}; j < goal_sequence.size(); ++j) {
        h_value += h_table.at(goal_sequence[j - 1]).at(goal_sequence[j]);
    }
    return h_value;
}

}  // namespace multi_a_star

}  // namespace cmapd