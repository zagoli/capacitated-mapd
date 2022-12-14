/**
 * @file
 * @brief The program entry point. It parses arguments and prints results. Speak, friend, and enter.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#include <fmt/color.h>
#include <fmt/format.h>

#include <argparse/argparse.hpp>
#include <filesystem>
#include <regex>
#include <string>

#include "CmapdSolution.h"
#include "Timer.hpp"
#include "ambient/AmbientMap.h"
#include "custom_types.h"
#include "generation/generate_instances.h"
#include "ortools/ortools.h"
#include "path_finders/cbs.h"
#include "path_finders/pp.h"

/**
 * Solves an instance, printing the solutions and execution times.
 * @param instances_path The path where the instance files are.
 * @param map_path The path to the map.
 * @param capacity The capacity of the agents.
 * @param solver The solver type, CBS or PBS.
 */
void solver(const std::filesystem::path& instances_path,
            const std::filesystem::path& map_path,
            int capacity,
            std::string_view solver);

/**
 * @brief The program entry point.
 */
int main(int argc, char* argv[]) {
    using namespace std::string_literals;
    argparse::ArgumentParser parser{"cmapd"};
    parser.add_description("Capacitated Multi Agent Pickup and Delivery solver.");
    parser.add_epilog("You must provide at least one option between generate or evaluate.");

    // --- Arguments to parse ---
    parser.add_argument("map_path")
        .help("The path pointing to the map file. For example, /home/map.txt");

    parser.add_argument("-g", "--generate")
        .help(
            "Flag used to specify you want to generate instances. "
            "If set you must provide the number of instances, agents and tasks.")
        .implicit_value(true)
        .default_value(false);

    parser.add_argument("-na", "--agents")
        .help("Specify the positive number of agents in the generated instances.")
        .metavar("N_AGENTS")
        .scan<'i', int>();

    parser.add_argument("-nt", "--tasks")
        .help("Specify the positive number of tasks in the generated instances.")
        .metavar("N_TASKS")
        .scan<'i', int>();

    parser.add_argument("-ni", "--instances")
        .help("Specify the positive number of instances to generate.")
        .metavar("N_INSTANCES")
        .scan<'i', int>();

    parser.add_argument("-o", "--instances-output")
        .help("Specify the path in which the output instances will be saved.")
        .metavar("OUT_PATH")
        .default_value("output"s);

    parser.add_argument("-e", "--evaluate")
        .help(
            "Specify the path pointing to a directory containing the generated instances "
            "and evaluate them.")
        .metavar("INSTANCES_PATH");

    parser.add_argument("-c", "--capacity")
        .help("The maximum number of task an agent can simultaneously carry.")
        .metavar("CAPACITY")
        .default_value(1)
        .scan<'i', int>();

    parser.add_argument("-s", "--solver")
        .help(
            "Specify the type of solver to use when evaluating the instances. "
            "Could be CBS or PP.")
        .metavar("SOLVER")
        .default_value("CBS"s);

    // --- Parsing arguments ---
    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << "\n";
        std::cerr << parser;
        std::exit(EXIT_FAILURE);
    }

    // --- Handling arguments ---
    if (parser["--generate"] == false && !parser.present("--evaluate")) {
        std::cerr << "You must provide at least one option between generate or evaluate!\n";
        std::exit(EXIT_FAILURE);
    }
    auto map_path = std::filesystem::path{parser.get("map_path")};

    if (parser["--generate"] == true) {
        auto instances_out_path = std::filesystem::path{parser.get("--instances-output")};
        auto n_agents_opt = parser.present<int>("--agents");
        auto n_tasks_opt = parser.present<int>("--tasks");
        auto n_instances_opt = parser.present<int>("--instances");
        if (n_agents_opt && n_tasks_opt && n_instances_opt) {
            std::cout << fmt::format(
                "Trying to generate {} instances with {} agents "
                "and {} tasks on map {} and saving them in {}...\n",
                n_instances_opt.value(),
                n_agents_opt.value(),
                n_tasks_opt.value(),
                map_path.string(),
                instances_out_path.string());

            cmapd::AmbientMap map{map_path};
            cmapd::generate_instances(map,
                                      instances_out_path,
                                      n_instances_opt.value(),
                                      n_agents_opt.value(),
                                      n_tasks_opt.value());
            std::cout << "Done." << std::endl;
        } else {
            std::cerr << "If you want to generate instances, you must provide the number "
                         "of instances, agents and tasks!\n";
            std::exit(EXIT_FAILURE);
        }
    }

    if (auto instances_in_path_opt = parser.present("--evaluate")) {
        auto instances_in_path = std::filesystem::path{instances_in_path_opt.value()};
        const std::string& solver_type = parser.get("--solver");
        const int capacity = parser.get<int>("--capacity");
        if (solver_type == "CBS" || solver_type == "PP") {
            std::cout << fmt::format(
                "Solving instances in {}, capacity set to {} with {} solver.\n",
                instances_in_path.string(),
                capacity,
                solver_type);
            solver(instances_in_path, map_path, capacity, solver_type);
        } else {
            std::cerr << solver_type
                      << " is not a known solver. Possible solvers are: CBS, PP (case "
                         "sensitive).\n";
            std::exit(EXIT_FAILURE);
        }
    }

    return 0;
}

/**
 * Prints a solution.
 * @param solution The solution to be printed.
 */
void print_solution(const cmapd::CmapdSolution& solution) {
    for (int i = 0; i < std::ssize(solution.paths); ++i) {
        std::cout << "Agent " << i << ": [";
        const char* padding = "";
        for (const auto& point : solution.paths[i]) {
            std::cout << padding << point;
            padding = ", ";
        }
        std::cout << "]" << std::endl;
    }
    fmt::print(
        fmt::emphasis::bold, "makespan:{:6}\ncost:{:10}\n", solution.makespan, solution.cost);
}

void solver(const std::filesystem::path& instances_path,
            const std::filesystem::path& map_path,
            int capacity,
            std::string_view solver) {
    using namespace cmapd;
    using namespace timer;

    Timer<HOST, seconds> T_HT;
    Timer<HOST, seconds> T_TA;
    Timer<HOST, seconds> T_PF;

    for (const auto& entry : std::filesystem::directory_iterator(instances_path)) {
        const auto filename = entry.path().filename().string();
        if (std::regex_match(filename, std::regex{"instance_[0-9]+\\.txt"})) {
            fmt::print(fmt::fg(fmt::color::light_green), "\nSolving {}\n", filename);
            try {
                T_HT.start();
                AmbientMapInstance instance{entry.path(), map_path};
                T_HT.stop();

                // Task assignment
                T_TA.start();
                std::vector<path_t> goal_sequences{assign_tasks(instance, capacity)};
                T_TA.stop();

                // Path finding
                CmapdSolution solution;
                T_PF.start();
                if (solver == "CBS") {
                    solution = cbs::cbs(instance, goal_sequences);
                } else if (solver == "PP") {
                    solution = pp::pp(instance, goal_sequences);
                }
                T_PF.stop();
                print_solution(solution);

                fmt::print(fmt::emphasis::bold,
                           "H-table computing time:{:20}'\n"
                           "Task assignment time:{:22}'\n"
                           "Path finding time:{:25}'\n",
                           T_HT.duration(),
                           T_TA.duration(),
                           T_PF.duration());

            } catch (const std::exception& ex) {
                std::cerr << "Instance skipped: " << ex.what() << std::endl;
            }
        }
    }

    auto total_time{T_HT.total_duration() + T_TA.total_duration() + T_PF.total_duration()};
    auto avg_time{T_HT.average() + T_TA.average() + T_PF.average()};
    fmt::print(fmt::fg(fmt::color::light_green),
               "\nTOTAL TIME:{:20} seconds\nMEAN TIME:{:21} seconds\n",
               total_time,
               avg_time);
}