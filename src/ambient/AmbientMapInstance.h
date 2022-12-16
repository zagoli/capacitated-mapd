/**
 * @file
 * @brief Contains the class AmbientMapInstance.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <filesystem>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Point.h"
#include "ambient/AmbientMap.h"
#include "custom_types.h"

namespace cmapd {
/**
 * @class AmbientMapInstance
 * @brief This class extends AmbientMap with the initial position of agents and tasks, and
 * with a distance matrix.
 */
class AmbientMapInstance final : public AmbientMap {
  private:
    std::vector<Point> m_agents;
    std::vector<std::pair<Point, Point>> m_tasks;
    h_table_t m_h_table;

  public:
    /**
     * Constructor of an already generated ambient map instance: takes two paths, one to
     * auto-generated file containing the instance info and the other to the map file use to
     * generate the previous one
     * @throw runtime_error if any of the required files do not exist
     */
    explicit AmbientMapInstance(const std::filesystem::path& path_to_map_instance,
                                const std::filesystem::path& path_to_map);
    /**
     * Constructor of ambient map instance: takes a map, a reference to a vector of agents
     * [std::vector<Point>] and a reference to a vector of tasks [std::vector<std::pair<Point,
     * Point>>]
     */
    explicit AmbientMapInstance(const AmbientMap& map,
                                const std::vector<Point>& a,
                                const std::vector<std::pair<Point, Point>>& t);
    /**
     * Method that return the number of agents in the map
     * @returns the number of agents in the map
     */
    [[nodiscard]] int num_agents() const;
    /**
     * Method that return the number of tasks in the map
     * @returns the number of tasks in the map
     */
    [[nodiscard]] int num_tasks() const;
    /**
     * Checks if a position can be a valid move position in the map
     * @param[in] p The point to check
     * @returns True if the given Point is inside the map and it's not a wall, false otherwise
     */
    [[nodiscard]] bool is_valid(Point p) const override;
    /**
     * Method that return a string representing the structure of the map
     * @returns a string representing the structure of the map
     */
    [[nodiscard]] std::string to_string() const override;
    /**
     * Method that return the list of tasks of the instance
     * @returns the list of tasks of the instance
     */
    [[nodiscard]] std::vector<std::pair<Point, Point>> tasks() const;
    /**
     * Method that returns the list of agents of the instance.
     * @return the list of agents of the instance.
     */
    [[nodiscard]] const std::vector<Point>& agents() const;
    /**
     * Method that returns the m_h_table of the instance.
     * @return the m_h_table
     */
    [[nodiscard]] const h_table_t& h_table() const;
    /**
     * Stream operator
     * @param os output stream
     * @param instance instance to send to stream
     * @return stream
     */
    friend std::ostream& operator<<(std::ostream& os, const AmbientMapInstance& instance);
};
}  // namespace cmapd

