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
/**
 * @class AmbientMapInstance
 * @brief Represents the Instance of a map to solve.
 */
namespace cmapd {
class AmbientMapInstance final : public AmbientMap {
  private:
    std::vector<Point> m_agents;
    std::vector<std::pair<Point, Point>> m_tasks;

  public:
    /**
     * Constructor of an already genetrated ambient map instace: takes two paths, one to
     * auto-generated file containing the instance info and the other to the map file use to
     * generate the previous one
     * @throw runtime_error if any of the required files do not exist
     */
    explicit AmbientMapInstance(const std::filesystem::path& path_to_map_instance,
                                const std::filesystem::path& path_to_map);
    /**
     * Constructor of ambient map instace: takes a map, a reference to a vector of agents
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
     * Checks if a position is valid in the map
     * @param[in] p The point to check
     * @returns True if the given Point is a valid position inside the map, false otherwise
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
     * Stream operator
     * @param os output stream
     * @param instance instance to send to stream
     * @return stream
     */
    friend std::ostream& operator<<(std::ostream& os, const AmbientMapInstance& instance);
};
}  // namespace cmapd

