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
#include <filesystem>
#include <string>

#include "ambient/AmbientMap.h"
#include "generation/generate_instances.h"

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
                "and {} tasks on map {} and saving them in {}...",
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
        const std::string& solver = parser.get("--solver");
        const int capacity = parser.get<int>("--capacity");
        // TODO: replace print with call to solver
        if (solver == "CBS") {
            std::cout << "Solving instances in " << instances_in_path << ", capacity " << capacity
                      << " with CBS solver.\n";
        } else if (solver == "PBS") {
            std::cout << "Solving instances in " << instances_in_path << ", capacity " << capacity
                      << " with PBS solver.\n";
        } else {
            std::cerr
                << solver
                << " is not a known solver. Possible solver are: CBS, PBS (case sensitive).\n";
            std::exit(EXIT_FAILURE);
        }
    }

    return 0;
}