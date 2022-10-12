//
// Created by Jacopo on 12/10/2022.
//
#include <argparse/argparse.hpp>
#include <string>

int main(int argc, char* argv[]) {
  argparse::ArgumentParser parser{"cmapd"};
  parser.add_description("Capacitated Multi Agent Pickup and Delivery solver.");

  parser.add_argument("map_path")
      .help("The absolute path pointing to the map file. For example, /home/map.txt");

  parser.add_argument("--generate")
      .help("Generate instances.")
      .default_value(false)
      .implicit_value(true);

  parser.add_argument("--agents")
      .help("Specify the positive number of agents in the generated instances.")
      .scan<'u', unsigned>();

  parser.add_argument("--tasks")
      .help("Specify the positive number of tasks in the generated instances.")
      .scan<'u', unsigned>();

  parser.add_argument("--instances-output")
      .help(
          "Specify the path in which the output instances will be saved. "
          "If missing, instances will be saved in a folder named 'output' "
          "next to the executable.")
      .default_value("output");

  parser.add_argument("--evaluate")
      .help(
          "Specify the path pointing to a directory containing the generated instances "
          "and evaluate them.");

  try {
    parser.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    std::exit(1);
  }

  return 0;
}