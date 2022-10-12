//
// Created by dade on 12/10/22.
//

#include "AmbientMap.h"

#include <filesystem>
#include <string>
#include <vector>

#include "Point.h"

namespace cmapd {

AmbientMap::AmbientMap(std::filesystem::path path_to_map) {
    //! TODO
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
    std::string s {};
    for(std::vector row : AmbientMap::grid){
        for(char c : row){
            //! TODO
        }
    }
}

}  // namespace cmapd