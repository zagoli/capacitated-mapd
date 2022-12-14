/**
 * @file
 * @brief Contains the implementation of assign_tasks.
 * @author Davide Furlani
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#include "ortools.h"

#include <vector>

#include "Point.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

namespace cmapd {

std::vector<path_t> assign_tasks(const AmbientMapInstance& instance, int capacity) {
    using namespace operations_research;

    // =============== SETTING STARTING AND ENDING NODES ===========================================

    // Defining the depot: every agent ends his route at this node.
    // Its internal value must be greater than all the other nodes in order to avoid conflicts
    const int depot_value{instance.num_agents() + instance.num_tasks() * 2};
    const RoutingIndexManager::NodeIndex depot{depot_value};

    // starting nodes of the agents
    std::vector<RoutingIndexManager::NodeIndex> starts{};
    for (int i = 0; i < instance.num_agents(); ++i) {
        starts.emplace_back(i);
    }
    // ending nodes of the agents: every agent ends at the depot
    std::vector<RoutingIndexManager::NodeIndex> ends{static_cast<size_t>(instance.num_agents()),
                                                     depot};

    // In the OR-Tools library, Nodes and indices don't correspond to real point on the map.
    // Therefore, we need to associate nodes with their real location.
    std::map<RoutingIndexManager::NodeIndex, Point> node_to_point{};
    std::map<Point, RoutingIndexManager::NodeIndex> point_to_node{};
    int counter = 0;
    // Can't use square brackets syntax for node_to_point since Point doesn't have default
    // constructor
    for (const auto& agent : instance.agents()) {
        node_to_point.emplace(RoutingIndexManager::NodeIndex{counter}, agent);
        point_to_node.emplace(agent, RoutingIndexManager::NodeIndex{counter});
        ++counter;
    }
    for (const auto& [start, end] : instance.tasks()) {
        node_to_point.emplace(RoutingIndexManager::NodeIndex{counter}, start);
        point_to_node.emplace(start, RoutingIndexManager::NodeIndex{counter});
        ++counter;

        node_to_point.emplace(RoutingIndexManager::NodeIndex{counter}, end);
        point_to_node.emplace(end, RoutingIndexManager::NodeIndex{counter});
        ++counter;
    }

    // =============== DEMANDS VECTOR ==============================================================

    // This vector associates every node with the weight picking up (or delivering) the task at that
    // node. The value is zero for the agents starting point, one for starting point of tasks, minus
    // one for ending point of tasks and zero for the depot.
    std::vector<int> demands(instance.num_agents() + 2 * instance.num_tasks() + 1, 0);
    for (int i = 0; i < instance.num_tasks(); ++i) {
        demands[instance.num_agents() + 2 * i] = 1;
        demands[instance.num_agents() + 2 * i + 1] = -1;
    }

    // =============== ROUTING OBJECTS =============================================================

    RoutingIndexManager manager{instance.num_agents() + instance.num_tasks() * 2
                                    + 1,  // the number of nodes: agents + tasks + depot
                                instance.num_agents(),  // the number of agents
                                starts,                 // starting nodes
                                ends};                  // ending nodes

    RoutingModel routing{manager};

    // =============== DISTANCE CALLBACK ===========================================================

    auto distance_callback = [&manager, &depot, &instance, &node_to_point](
                                 int64_t from_index, int64_t to_index) -> int64_t {
        auto from_node = manager.IndexToNode(from_index);
        auto to_node = manager.IndexToNode(to_index);
        // the distance from all nodes to the depot is zero
        if (to_node == depot) {
            return 0;
        }
        auto from_point = node_to_point.at(from_node);
        auto to_point = node_to_point.at(to_node);
        return instance.h_table().at(from_point).at(to_point);
    };

    auto transit_callback_index{routing.RegisterTransitCallback(distance_callback)};
    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

    auto dimension_name{"Distance"};
    routing.AddDimension(
        transit_callback_index, 0, std::numeric_limits<int64_t>::max(), true, dimension_name);
    auto distance_dimension = routing.GetMutableDimension("Distance");
    distance_dimension->SetGlobalSpanCostCoefficient(100);

    // =============== DEMANDS CALLBACK ============================================================

    auto demand_callback = [&demands, &manager](int64_t from_index) -> int64_t {
        int from_node = manager.IndexToNode(from_index).value();
        return demands.at(from_node);
    };
    const int demand_callback_index{routing.RegisterUnaryTransitCallback(demand_callback)};

    routing.AddDimension(demand_callback_index, int16_t(0), capacity, true, "Capacity");

    // =============== SETTING UP PICKUP AND DELIVERIES OF TASKS ===================================

    Solver* const solver = routing.solver();

    for (const auto& [start, end] : instance.tasks()) {
        int64_t pickup_index = manager.NodeToIndex(point_to_node.at(start));
        int64_t delivery_index = manager.NodeToIndex(point_to_node.at(end));
        routing.AddPickupAndDelivery(pickup_index, delivery_index);
        solver->AddConstraint(solver->MakeEquality(routing.VehicleVar(pickup_index),
                                                   routing.VehicleVar(delivery_index)));
        solver->AddConstraint(
            solver->MakeLessOrEqual(distance_dimension->CumulVar(pickup_index),
                                    distance_dimension->CumulVar(delivery_index)));
    }

    // =============== CALLING THE LIBRARY SOLVER ==================================================

    auto search_parameters{DefaultRoutingSearchParameters()};
    search_parameters.set_first_solution_strategy(
        FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION);

    const Assignment* solution{routing.SolveWithParameters(search_parameters)};

    if (solution == nullptr) {
        throw std::runtime_error("[ortools] No solution found");
    }

    // =============== CONSTRUCTING THE GOAL SEQUENCES FROM THE SOLUTION ===========================

    std::vector<path_t> goal_sequences{};
    for (int agent = 0; agent < instance.num_agents(); ++agent) {
        auto index{routing.Start(agent)};
        path_t path{};
        while (!routing.IsEnd(index)) {
            path.emplace_back(node_to_point.at(manager.IndexToNode(index)));
            index = solution->Value(routing.NextVar(index));
        }
        goal_sequences.push_back(path);
    }

    return goal_sequences;  // Ciccio Spastic
}

}  // namespace cmapd
