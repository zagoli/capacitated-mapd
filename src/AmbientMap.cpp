//
// Created by dade on 12/10/22.
//

#include "AmbientMap.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "Point.h"

namespace cmapd {

bool is_valid_char(char c){
    return c=='#' || c==' ' || c=='O';
}
// TODO: controlla che il file sia formattato correttamente
AmbientMap::AmbientMap(const std::filesystem::path& path_to_map) {
    std::ifstream map_file{path_to_map};
    if (!map_file) {
        throw std::runtime_error{path_to_map.string() + ": file does non exist"};
    }
    size_t rows {};
    map_file >> rows;
    size_t cols {};
    map_file >> cols;
    for (size_t r = 0; r < rows; r++){
        char ch {};
        map_file.read(&ch, sizeof(char)); //discard \n
        AmbientMap::grid.push_back(*new std::vector<char>);
        for (size_t c = 0; c < cols; c++) {
            map_file.read(&ch, sizeof(char));
            if(is_valid_char(ch))
            AmbientMap::grid[r].push_back(ch);
        }
    }
}

const std::vector<std::vector<char>>& AmbientMap::get_map() const{
        return AmbientMap::grid;
}

int AmbientMap::get_rows() const {
    if (grid.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of rows is larger than a int.");
    return static_cast<int>(AmbientMap::grid.size());
}

int AmbientMap::get_columns() const {
    if (grid[0].size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of columns is larger than a int.");
    return static_cast<int>(AmbientMap::grid[0].size());
}

bool AmbientMap::is_valid_position(Point p) const {
    return p.row >= 0 && p.row < this->get_rows() && p.col >= 0 && p.col < this->get_columns();
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