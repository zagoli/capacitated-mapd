/**
 * @file
 * @brief Contains the class AmbientMap.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#pragma once
#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

#include "Point.h"

namespace cmapd {
/**
 * @class AmbientMap
 * @brief This class represents a grid, on which agents and tasks move.
 */
class AmbientMap {
  protected:
    /**
     * The parsed map
     */
    std::vector<std::vector<char>> m_grid;

  public:
    /**
     * Constructor of ambient map: take a path to file containing 2 integers representing number of
     * rows and columns of the map and the overall structure of the map ('#' for walls, ' ' for
     * empty spaces, 'O' for possible agent and targets positions)
     * @param path_to_map
     * @throw runtime_error if the file in input does not exist
     */
    explicit AmbientMap(const std::filesystem::path& path_to_map);
    /**
     * Method that returns a const reference of the map structure
     * @return const reference of the map structure
     */
    [[nodiscard]] const std::vector<std::vector<char>>& map() const;
    /**
     * Method that return an int representing the number of rows of the map
     * @return the number of rows of the map
     */
    [[nodiscard]] int rows_number() const;
    /**
     * Method that return an int representing the number of columns of the map
     * @return the number of columns of the map
     */
    [[nodiscard]] int columns_number() const;
    /**
     * Checks if a position is inside in the map
     * @param p the point to check
     * @return true if the given Point is inside the map, false otherwise
     */
    [[nodiscard]] bool is_inside(Point p) const;
    /**
     * Checks if a position is valid to generate agents or tasks in the map
     * @param p the point to check
     * @return true if the given Point is a possible generation position inside the map, false
     * otherwise
     */
    [[nodiscard]] virtual bool is_valid(Point p) const;
    /**
     * Method that return a string representing the structure of the map
     * @return a string representing the structure of the map
     */
    [[nodiscard]] virtual std::string to_string() const;
    /**
     * Stream operator
     * @param os output stream
     * @param map map to send to stream
     * @return stream
     */
    friend std::ostream& operator<<(std::ostream& os, const AmbientMap& map);
};

}  // namespace cmapd
