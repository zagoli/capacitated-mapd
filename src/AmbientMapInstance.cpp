//
// Created by dade on 20/10/22.
//

#include "AmbientMapInstance.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Point.h"

namespace cmapd {


AmbientMapInstance::AmbientMapInstance(const std::filesystem::path& path_to_map, const std::filesystem::path& path_to_map_instance) : AmbientMap(path_to_map) {
    
    //std::ifstream map_file{std::filesystem::absolute(path_to_map)};
    std::ifstream map_instance_file{path_to_map_instance};
    if (!map_instance_file) {
        throw std::runtime_error{path_to_map_instance.string() + ": file does non exist"};
    }
    
    int num_agents {0};
    int num_tasks {0};
    
    map_instance_file >> num_agents;
    map_instance_file >> num_tasks;
    
    for(int i=0; i<num_agents; i++){
        int row_pos {0};
        int col_pos {0};
        map_instance_file >> row_pos;
        map_instance_file >> col_pos;
        grid[row_pos][col_pos] = 'a';
        agents.push_back(Point{row_pos, col_pos});
    }
    
    for(int i=0; i<num_tasks; i++){
        int row_pos_start {0};
        int col_pos_start {0};
        int row_pos_goal {0};
        int col_pos_goal {0};
        map_instance_file >> row_pos_start;
        map_instance_file >> col_pos_start;
        grid[row_pos_start][col_pos_start] = 'o';
        map_instance_file >> row_pos_goal;
        map_instance_file >> col_pos_goal;
        grid[row_pos_goal][col_pos_goal] = 'o';
        tasks.emplace_back(Point{row_pos_start, col_pos_start}, Point{row_pos_goal, col_pos_goal});
    }
}

int AmbientMapInstance::get_num_agents() const {
    if (agents.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of agents is larger than a int.");
    return static_cast<int>(agents.size());
}

int AmbientMapInstance::get_num_tasks() const {
    if (tasks.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of tasks is larger than a int.");
    return static_cast<int>(tasks.size());
}

std::string AmbientMapInstance::to_string() const {
    std::string s;
    for (int r=0; r<this->get_rows_number(); r++) {
        for (int c=0; c<this->get_columns_number(); c++) {
            if(grid[r][c] == 'O')
                s += ' ';
            else
                s += grid[r][c];
        }
        s += "\n";
    }
    return s;
}
}  // namespace cmapd
