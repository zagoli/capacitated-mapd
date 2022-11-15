/**
 * @file
 * @brief Contains the implementation of class AmbientMap.
 * @author Davide Furlani
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "ambient/AmbientMap.h"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <vector>

#include "Point.h"

namespace cmapd {

/**
 * Checks if a char is valid or not.
 * @param c The char to be checked.
 * @param row The row of the char.
 * @param col The column of the char.
 */
void validate_char(char c, int row, int col) {
    if (c != '#' && c != ' ' && c != 'O') {
        throw std::runtime_error{
            fmt::format("character {} in line {}:{} is not a valid character", c, row, col)};
    }
}

AmbientMap::AmbientMap(const std::filesystem::path& path_to_map) {
    
    std::ifstream map_file{path_to_map};
    if (!map_file) {
        throw std::runtime_error{fmt::format("{}: file does non exist", path_to_map.string())};
    }
    
    std::string line {};
    int rows_counter {0};
    while(std::getline(map_file, line)){
        std::vector<char> char_vec {};
        int cols_counter {0};
        for(char c : line){
            validate_char(c, rows_counter, cols_counter);
            char_vec.push_back(c);
            cols_counter++;
        }
        rows_counter++;
        AmbientMap::m_grid.emplace_back(char_vec);
    }
}

const std::vector<std::vector<char>>& AmbientMap::map() const { return AmbientMap::m_grid; }

int AmbientMap::rows_number() const {
    if (m_grid.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of rows is larger than a int.");
    return static_cast<int>(m_grid.size());
}

int AmbientMap::columns_number() const {
    if (m_grid[0].size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of columns is larger than a int.");
    return static_cast<int>(m_grid[0].size());
}

bool AmbientMap::is_inside(Point p) const {
    return p.row >= 0 && p.row < this->rows_number() && p.col >= 0
           && p.col < this->columns_number();
}

bool AmbientMap::is_valid(Point p) const {
    return this->is_inside(p) && m_grid[p.row][p.col] == 'O';
}

std::string AmbientMap::to_string() const {
    std::string s;
    for (const auto& row : AmbientMap::m_grid) {
        for (char c : row) {
            s += c;
        }
        s += "\n";
    }
    return s;
}
std::ostream& operator<<(std::ostream& os, const AmbientMap& map) {
    os << map.to_string();
    return os;
}

}  // namespace cmapd