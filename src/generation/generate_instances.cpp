//
// Created by dade on 25/10/22.
//

#include "generate_instances.h"

#include <fstream>
#include <iostream>
#include <random>

#include "fmt/format.h"

namespace cmapd {

std::vector<AmbientMapInstance> generate_instances(const AmbientMap& map,
                                                   std::filesystem::path save_path,
                                                   int n_instances,
                                                   int n_agents,
                                                   int n_tasks) {
    if (n_instances <= 0) {
        throw std::runtime_error("Number of instances to generate must be grater than zero");
    }

    std::vector<Point> agents{};
    std::vector<std::pair<Point, Point>> tasks{};
    std::vector<AmbientMapInstance> instances{};

    std::vector<Point> possible_positions{};
    auto grid = map.get_map();
    for (int i = 0; i < static_cast<int>(grid.size()); i++) {
        for (int j = 0; j < static_cast<int>(grid[i].size()); j++) {
            if (grid[i][j] == 'O') possible_positions.emplace_back(Point{i, j});
        }
    }

    if (static_cast<int>(possible_positions.size()) < n_agents + 2 * n_tasks) {
        throw std::runtime_error(
            "Number of agents and tasks are more than possible generation places\n "
            "try reduce number of agents or task, or increase the number of generation places");
    }

    std::random_device seeder;
    const auto seed{static_cast<bool>(seeder.entropy()) ? seeder() : time(nullptr)};
    std::mt19937 engine{static_cast<std::mt19937::result_type>(seed)};

    for (int i = 0; i < n_instances; i++) {
        std::vector<Point> copy_possible_positions{possible_positions};
        for (int j = 0; j < n_agents; j++) {
            std::uniform_int_distribution<int> distribution{
                0, static_cast<int>(copy_possible_positions.size() - 1)};
            int the_chosen_one = distribution(engine);
            agents.emplace_back(copy_possible_positions[the_chosen_one]);
            copy_possible_positions.erase(copy_possible_positions.begin() + the_chosen_one);
        }
        for (int j = 0; j < n_tasks; j++) {
            std::uniform_int_distribution<int> distribution{
                0, static_cast<int>(copy_possible_positions.size() - 1)};
            int the_chosen_one = distribution(engine);
            Point the_chosen_start{copy_possible_positions[the_chosen_one]};
            copy_possible_positions.erase(copy_possible_positions.begin() + the_chosen_one);

            distribution = std::uniform_int_distribution<int>{
                0, static_cast<int>(copy_possible_positions.size() - 1)};
            int the_chosen_two = distribution(engine);
            Point the_chosen_end{copy_possible_positions[the_chosen_two]};
            copy_possible_positions.erase(copy_possible_positions.begin() + the_chosen_two);

            tasks.emplace_back(std::pair{the_chosen_start, the_chosen_end});
        }
        instances.emplace_back(AmbientMapInstance{map, agents, tasks});

        std::string nome_file = fmt::format("instance_{}.txt", i);
        std::filesystem::path absolute_path = absolute(std::filesystem::path(save_path));
        std::filesystem::create_directory(absolute_path);

        std::ofstream out_file{absolute_path.string() + "/" + nome_file};

        out_file << agents.size() << " " << tasks.size() << std::endl;
        for (Point a : agents) out_file << a.row << " " << a.col << std::endl;
        for (std::pair<Point, Point> t : tasks)
            out_file << t.first.row << " " << t.first.col << " " << t.second.row << " "
                     << t.second.col << std::endl;

        out_file.close();
        agents.clear();
        tasks.clear();
    }

    return instances;
}
}  // namespace cmapd