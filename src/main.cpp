/**
 * @file main.cpp
 * @brief The program entry point. It mainly parses arguments and prints results. Speak, friend, and
 * enter.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */
#include <fmt/format.h>

#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <regex>
#include <string>

#include "CmapdSolution.h"
#include "ambient/AmbientMap.h"
#include "custom_types.h"
#include "generation/generate_instances.h"
#include "ortools/ortools.h"
#include "path_finders/cbs.h"

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

int main(int argc, char* argv[]) {
    using namespace std::string_literals;
    argparse::ArgumentParser parser{"cmapd"};
    parser.add_description("Capacitated Multi Agent Pickup and Delivery solver.");
    parser.add_epilog("You must provide at least one option between generate or evaluate.");

    // --- Arguments to parse ---
    parser.add_argument("map_path")
        .help("The path pointing to the map file. For example, /home/map.txt");

    parser.add_argument("--generate")
        .help(
            "Flag used to specify you want to generate instances. "
            "If set you must provide the number of instances, m_agents and m_tasks.")
        .implicit_value(true)
        .default_value(false);

    parser.add_argument("--m_agents")
        .help("Specify the positive number of m_agents in the generated instances.")
        .metavar("N_AGENTS")
        .scan<'i', int>();

    parser.add_argument("--m_tasks")
        .help("Specify the positive number of m_tasks in the generated instances.")
        .metavar("N_TASKS")
        .scan<'i', int>();

    parser.add_argument("--instances")
        .help("Specify the positive number of instances to generate.")
        .metavar("N_INSTANCES")
        .scan<'i', int>();

    parser.add_argument("--instances-output")
        .help("Specify the path in which the output instances will be saved.")
        .metavar("OUT_PATH")
        .default_value("output"s);

    parser.add_argument("--evaluate")
        .help(
            "Specify the path pointing to a directory containing the generated instances "
            "and evaluate them.")
        .metavar("INSTANCES_PATH");

    parser.add_argument("--capacity")
        .help("The maximum number of task an agent can simultaneously carry.")
        .metavar("CAPACITY")
        .default_value(1)
        .scan<'i', int>();

    parser.add_argument("--solver")
        .help(
            "Specify the type of solver to use when evaluating the instances. "
            "Could be CBS or PBS.")
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
        auto n_agents_opt = parser.present<int>("--m_agents");
        auto n_tasks_opt = parser.present<int>("--m_tasks");
        auto n_instances_opt = parser.present<int>("--instances");
        if (n_agents_opt && n_tasks_opt && n_instances_opt) {
            std::cout << fmt::format(
                "Trying to generate {} instances with {} agents "
                "and {} tasks on map {} and saving them in {}...\n",
                n_agents_opt.value(),
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
                         "of instances, m_agents and m_tasks!\n";
            std::exit(EXIT_FAILURE);
        }
    }

    if (auto instances_in_path_opt = parser.present("--evaluate")) {
        auto instances_in_path = std::filesystem::path{instances_in_path_opt.value()};
        const std::string& solver_type = parser.get("--solver_type");
        const int capacity = parser.get<int>("--capacity");
        if (solver_type == "CBS" || solver_type == "PBS") {
            std::cout << fmt::format(
                "Solving instances in {} , capacity set to {}  with {} solver_type.\n",
                instances_in_path.string(),
                capacity,
                solver_type);
            solver(instances_in_path, map_path, capacity, solver_type);
        } else {
            std::cerr << solver_type
                      << " is not a known solver_type. Possible solver_type are: CBS, PBS (case "
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
    std::cout << fmt::format("makespan:{:7}, cost:{:7}\n", solution.makespan, solution.cost);
    for (const auto& path : solution.paths) {
        const char* padding = "";
        for (const auto& point : path) {
            std::cout << padding << point;
            padding = ", ";
        }
        std::cout << std::endl;
    }
}

void solver(const std::filesystem::path& instances_path,
            const std::filesystem::path& map_path,
            int capacity,
            std::string_view solver) {
    using namespace cmapd;
    using std::chrono::high_resolution_clock;
    using std::chrono::seconds;
    for (const auto& entry : std::filesystem::directory_iterator(instances_path)) {
        const auto filename = entry.path().filename().string();
        if (std::regex_match(filename, std::regex{"instance_[0-9]+\\.txt"})) {
            std::cout << "Solving " << filename << std::endl;
            try {
                auto time_before = high_resolution_clock::now();
                AmbientMapInstance instance{entry.path(), map_path};
                auto time_after = high_resolution_clock::now();
                auto h_table_time{
                    std::chrono::duration_cast<seconds>(time_before - time_after).count()};

                // Task assignment
                time_before = high_resolution_clock::now();
                std::vector<path_t> goal_sequences{assign_tasks(instance, capacity)};
                time_after = high_resolution_clock::now();
                auto task_assignment_time{
                    std::chrono::duration_cast<seconds>(time_before - time_after).count()};

                // Path finding
                long path_finder_time;
                if (solver == "CBS") {
                    time_before = high_resolution_clock::now();
                    CmapdSolution solution{cbs::cbs(instance, goal_sequences)};
                    time_after = high_resolution_clock::now();
                    path_finder_time
                        = std::chrono::duration_cast<seconds>(time_before - time_after).count();

                    print_solution(solution);
                } else if (solver == "PBS") {
                    time_before = high_resolution_clock::now();
                    time_after = high_resolution_clock::now();
                    path_finder_time
                        = std::chrono::duration_cast<seconds>(time_before - time_after).count();
                }

                std::cout << fmt::format(
                    "H-table computing time:{:10}\n"
                    "Task assignment time:{:12}\n"
                    "Path finding time:{:15}\n",
                    h_table_time,
                    task_assignment_time,
                    path_finder_time);

            } catch (const std::exception& ex) {
                std::cerr << "Instance skipped: " << ex.what() << std::endl;
            }
        }
    }
}