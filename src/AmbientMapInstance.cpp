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

AmbientMapInstance::AmbientMapInstance(const std::filesystem::path& path_to_map_instance,
                                       const std::filesystem::path& path_to_map)
    : AmbientMap(path_to_map) {
    // std::ifstream map_file{std::filesystem::absolute(path_to_map)};
    std::ifstream map_instance_file{path_to_map_instance};
    if (!map_instance_file) {
        throw std::runtime_error{path_to_map_instance.string() + ": file does non exist"};
    }

    int num_agents{0};
    int num_tasks{0};

    map_instance_file >> num_agents;
    map_instance_file >> num_tasks;

    for (int i = 0; i < num_agents; i++) {
        int row_pos{0};
        int col_pos{0};
        map_instance_file >> row_pos;
        map_instance_file >> col_pos;
        m_grid[row_pos][col_pos] = 'a';
        m_agents.push_back(Point{row_pos, col_pos});
    }

    for (int i = 0; i < num_tasks; i++) {
        int row_pos_start{0};
        int col_pos_start{0};
        int row_pos_goal{0};
        int col_pos_goal{0};
        map_instance_file >> row_pos_start;
        map_instance_file >> col_pos_start;
        m_grid[row_pos_start][col_pos_start] = 't';
        map_instance_file >> row_pos_goal;
        map_instance_file >> col_pos_goal;
        m_grid[row_pos_goal][col_pos_goal] = 't';
        m_tasks.emplace_back(Point{row_pos_start, col_pos_start},
                             Point{row_pos_goal, col_pos_goal});
    }
}

int AmbientMapInstance::get_num_agents() const {
    if (m_agents.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of m_agents is larger than a int.");
    return static_cast<int>(m_agents.size());
}

int AmbientMapInstance::get_num_tasks() const {
    if (m_tasks.size() > std::numeric_limits<int>::max())
        throw std::overflow_error("The number of m_tasks is larger than a int.");
    return static_cast<int>(m_tasks.size());
}

bool AmbientMapInstance::is_valid_position(Point p) const {
    return p.row >= 0 && p.row < this->get_rows_number() && p.col >= 0
           && p.col < this->get_columns_number() && m_grid[p.row][p.col] != '#';
}

std::string AmbientMapInstance::to_string() const {
    std::string s;
    for (int r = 0; r < this->get_rows_number(); r++) {
        for (int c = 0; c < this->get_columns_number(); c++) {
            if (m_grid[r][c] == 'O')
                s += ' ';
            else
                s += m_grid[r][c];
        }
        s += "\n";
    }
    return s;
}

std::vector<std::pair<Point, Point>> AmbientMapInstance::get_tasks() const { return m_tasks; }
}  // namespace cmapd
