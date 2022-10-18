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
 * @brief Represents the ambient where agents will move.
 */
namespace cmapd {
class AmbientMap {
  private:
    /// The parsed map
    std::vector<std::vector<char>> grid;

  public:
    /// Constructor of Ambient map: take a path to file containing 2 integers representing number of rows and columns of the map and the overall structure of the map ('#' for walls, ' ' for empty spaces, 'O' for possible agent and targets positions)
    explicit AmbientMap(const std::filesystem::path& path_to_map);
    /// Method that returns a const reference of the map structure
    /// @returns const reference of the map structure
    [[nodiscard]] const std::vector<std::vector<char>>& get_map();
    /// Method that return a size_t representing the number of rows of the map
    /// @returns the number of rows of the map
    [[nodiscard]] size_t get_rows() const;
    /// Method that return a size_t representing the number of columns of the map
    /// @returns the number of columns of the map
    [[nodiscard]] size_t get_columns() const;
    /// Checks if a position is valid in the map
    /// @param[in] p The point to check
    /// @returns True if the given Point is a valid position inside the map, false otherwise
    [[nodiscard]] bool is_valid_position(Point p) const;
    /// Method that return a string representing the structure of the map
    /// @returns a string representing the structure of the map
    [[nodiscard]] std::string to_string() const;
};

}  // namespace cmapd
