//
// Created by dade on 12/10/22.
//

#include "AmbientMap.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Point.h"

#include <fmt/format.h>

namespace cmapd {

bool is_valid_char(char c, int row, int col){
    if (c == '#' || c == ' ' || c == 'O') {
        return true;
    } else {
        throw std::runtime_error{fmt::format("character {} in line {}:{} is not a valid character", c, row, col)};
    }
}


AmbientMap::AmbientMap(const std::filesystem::path& path_to_map) {
    
    //std::ifstream map_file{std::filesystem::absolute(path_to_map)};
    std::ifstream map_file{path_to_map};
    if (!map_file) {
        throw std::runtime_error{path_to_map.string() + ": file does non exist"};
    }
    
    std::string line {};
    int rows_counter {0};
    while(std::getline(map_file, line)){
        rows_counter++;
        std::vector<char> char_vec {};
        int cols_counter {0};
        for(char c : line){
            cols_counter++;
            if(is_valid_char(c, rows_counter, cols_counter))
                char_vec.push_back(c);
        }
        AmbientMap::grid.emplace_back(char_vec);
    }
}

const std::vector<std::vector<char>>& AmbientMap::get_map() const{
        return AmbientMap::grid;
}

int AmbientMap::get_rows_number() const {
    if (grid.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of rows is larger than a int.");
    return static_cast<int>(AmbientMap::grid.size());
}

int AmbientMap::get_columns_number() const {
    if (grid[0].size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of columns is larger than a int.");
    return static_cast<int>(AmbientMap::grid[0].size());
}

bool AmbientMap::is_valid_position(Point p) const {
    return p.row >= 0 && p.row < this->get_rows_number() 
           && p.col >= 0 && p.col < this->get_columns_number()
           && grid[p.row][p.col] != '#';
}

std::string AmbientMap::to_string() const {
    std::string s;
    for (const auto& row : AmbientMap::grid) {
        for (char c : row) {
            s += c;
        }
        s += "\n";
    }
    return s;
}

}  // namespace cmapd