/**
 * @file AmbientMap.h
 * @brief Contains the class AmbientMap.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once
#include <Point.h>

#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

#include "AmbientMap.h"
#include "custom_types.h"
/**
 * @class AmbientMapInstance
 * @brief Represents the Instance of a map to solve.
 */
namespace cmapd {
class AmbientMapInstance final : public AmbientMap {
  private:
    std::vector<Point> m_agents;
    std::vector<std::pair<Point, Point>> m_tasks;
    h_table_t h_table;

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
    [[nodiscard]] int get_num_agents() const;
    /**
     * Method that return the number of tasks in the map
     * @returns the number of tasks in the map
     */
    [[nodiscard]] int get_num_tasks() const;
    /**
     * Checks if a position can be a valid move position in the map
     * @param[in] p The point to check
     * @returns True if the given Point is inside the map and it's not a wall, false otherwise
     */
    [[nodiscard]] bool is_valid_position(Point p) const override;
    /**
     * Method that return a string representing the structure of the map
     * @returns a string representing the structure of the map
     */
    [[nodiscard]] std::string to_string() const override;
    /**
     * Method that return the list of tasks of the instance
     * @returns the list of tasks of the instance
     */
    [[nodiscard]] std::vector<std::pair<Point, Point>> get_tasks() const;
    /**
     * Method that returns the list of agents of the instance.
     * @return the list of agents of the instance.
     */
    [[nodiscard]] const std::vector<Point>& get_agents() const;
    /**
     * Method that returns the h_table of the instance.
     * @return the h_table
     */
    [[nodiscard]] const h_table_t& get_h_table() const;
    /**
     * Stream operator
     * @param os output stream
     * @param instance instance to send to stream
     * @return stream
     */
    friend std::ostream& operator<<(std::ostream& os, const AmbientMapInstance& instance);
    /**
     * Method that modify the map to add a wall in a given point in the map
     * @param p the point to tranform to a wall
     */
    void wall(Point p);
};
}  // namespace cmapd

