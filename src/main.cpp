//
// Created by Jacopo on 12/10/2022.
//
#include <argparse/argparse.hpp>
#include <filesystem>
#include <string>

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
            "If set you must provide the number of instances, agents and tasks.")
        .implicit_value(true)
        .default_value(false);

    parser.add_argument("--agents")
        .help("Specify the positive number of agents in the generated instances.")
        .metavar("N_AGENTS")
        .scan<'u', unsigned>();

    parser.add_argument("--tasks")
        .help("Specify the positive number of tasks in the generated instances.")
        .metavar("N_TASKS")
        .scan<'u', unsigned>();

    parser.add_argument("--instances")
        .help("Specify the positive number of instances to generate.")
        .metavar("N_INSTANCES")
        .scan<'u', unsigned>();

    parser.add_argument("--instances-output")
        .help("Specify the path in which the output instances will be saved.")
        .metavar("OUT_PATH")
        .default_value("output"s);

    parser.add_argument("--evaluate")
        .help(
            "Specify the path pointing to a directory containing the generated instances "
            "and evaluate them.")
        .metavar("INSTANCES_PATH");

    parser.add_argument("--solver")
        .help("Specify the type of solver to use when evaluating the instances. "
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
        auto n_agents_opt = parser.present<unsigned>("--agents");
        auto n_tasks_opt = parser.present<unsigned>("--tasks");
        auto n_instances_opt = parser.present<unsigned>("--instances");
        if (n_agents_opt && n_tasks_opt && n_instances_opt) {
            // TODO: replace print with call to generate instances.
            std::cout << "Generating " << n_instances_opt.value() << " instances with "
                      << n_agents_opt.value() << " agents and " << n_tasks_opt.value()
                      << " tasks on map " << map_path << " and saving them in "
                      << instances_out_path << std::endl;
        } else {
            std::cerr << "If you want to generate instances, you must provide the number "
                         "of instances, agents and tasks!\n";
            std::exit(EXIT_FAILURE);
        }
    }
    if (auto instances_in_path_opt = parser.present("--evaluate")) {
        auto instances_in_path = std::filesystem::path{instances_in_path_opt.value()};
        const std::string& solver = parser.get("--solver");
        // TODO: replace print with call to solver
        if (solver == "CBS") {
            std::cout << "Solving instances in " << instances_in_path << " with CBS solver.\n";
        } else if (solver == "PBS") {
            std::cout << "Solving instances in " << instances_in_path << " with PBS solver.\n";
        } else {
            std::cerr << solver << " is not a known solver. Possible solver are: CBS, PBS (case sensitive).\n";
            std::exit(EXIT_FAILURE);
        }
    }

    return 0;
}