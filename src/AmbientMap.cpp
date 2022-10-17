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

AmbientMap::AmbientMap(const std::filesystem::path& path_to_map) {
    std::ifstream map_file{path_to_map};
    if (!map_file) {
        throw std::runtime_error{path_to_map.string() + ": file does non exist"};
    }
    size_t rows{};
    map_file >> rows;
    size_t cols{};
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

size_t AmbientMap::get_rows() const{
    return AmbientMap::grid.size();
}

size_t AmbientMap::get_columns() const{
    return AmbientMap::grid[0].size();
}

bool AmbientMap::is_valid_position(Point p) const{
    return p.row<AmbientMap::grid.size() && p.col<AmbientMap::grid[0].size();
}

std::string AmbientMap::to_string() const{
    std::string s {""};
    for(std::vector row : AmbientMap::grid){
        for(char c : row){
            s += c;
        }
        s += "\n";
    }
    return s;
}

}  // namespace cmapd