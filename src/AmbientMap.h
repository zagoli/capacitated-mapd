//
// Created by dade on 12/10/22.
//
/**
 * @file AmbientMap.h
 * @brief Contains the class AmbientMap.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "Point.h"
/**
 * @class AmbientMap
 * @brief Represents the ambient where m_agents will move.
 */
namespace cmapd {
class AmbientMap {
  protected:
    /// The parsed map
    std::vector<std::vector<char>> m_grid;

  public:
    /// Constructor of Ambient map: take a path to file containing 2 integers representing number of rows and columns of the map and the overall structure of the map ('#' for walls, ' ' for empty spaces, 'O' for possible agent and targets positions)
    /// @throw runtime_error if the file in input does not exist
    explicit AmbientMap(const std::filesystem::path& path_to_map);
    /// Method that returns a const reference of the map structure
    /// @returns const reference of the map structure
    [[nodiscard]] const std::vector<std::vector<char>>& get_map() const;
    /// Method that return an int representing the number of rows of the map
    /// @returns the number of rows of the map
    [[nodiscard]] int get_rows_number() const;
    /// Method that return an int representing the number of columns of the map
    /// @returns the number of columns of the map
    [[nodiscard]] int get_columns_number() const;
    /// Checks if a position is valid in the map
    /// @param[in] p The point to check
    /// @returns True if the given Point is a valid position inside the map, false otherwise
    [[nodiscard]] virtual bool is_valid_position(Point p) const;
    /// Method that return a string representing the structure of the map
    /// @returns a string representing the structure of the map
    [[nodiscard]] virtual std::string to_string() const;
};

}  // namespace cmapd
