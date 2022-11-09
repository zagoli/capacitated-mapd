//
// Created by dade on 04/11/22.
//

#include "ortools.h"

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

namespace cmapd {

std::vector<path_t> assign_tasks(const AmbientMapInstance& instance,
                                 int capacity,
                                 const h_table_t& h_table) {
    using namespace operations_research;

    // =============== SETTING STARTING AND ENDING NODES ===========================================

    // Defining the depot: every agent ends his route at this node.
    // Its internal value must be greater than all the other nodes in order to avoid conflicts
    const int depot_value{instance.get_num_agents() + instance.get_num_tasks() * 2};
    const RoutingIndexManager::NodeIndex depot{depot_value};

    // starting nodes of the agents
    std::vector<RoutingIndexManager::NodeIndex> starts{};
    for (int i = 0; i < instance.get_num_agents(); ++i) {
        starts.emplace_back(i);
    }
    // ending nodes of the agents: every agent ends at the depot
    std::vector<RoutingIndexManager::NodeIndex> ends{static_cast<size_t>(instance.get_num_agents()),
                                                     depot};

    // In the OR-Tools library, Nodes and indices don't correspond to real point on the map.
    // Therefore, we need to associate nodes with their real location.
    std::map<RoutingIndexManager::NodeIndex, int64_t> node_to_ravel_point{};
    std::map<Point, RoutingIndexManager::NodeIndex> point_to_node{};
    int counter = 0;
    for (const auto& agent : instance.get_agents()) {
        node_to_ravel_point[RoutingIndexManager::NodeIndex{counter}] = instance.ravel(agent);
        point_to_node[agent] = RoutingIndexManager::NodeIndex{counter};
        ++counter;
    }
    for (const auto& [start, end] : instance.get_tasks()) {
        node_to_ravel_point[RoutingIndexManager::NodeIndex{counter}] = instance.ravel(start);
        point_to_node[start] = RoutingIndexManager::NodeIndex{counter};
        ++counter;

        node_to_ravel_point[RoutingIndexManager::NodeIndex{counter}] = instance.ravel(end);
        point_to_node[end] = RoutingIndexManager::NodeIndex{counter};
        ++counter;
    }

    // =============== DEMANDS VECTOR ==============================================================

    // This vector associates every node with the weight picking up (or delivering) the task at that
    // node. The value is zero for the agents starting point, one for starting point of tasks, minus
    // one for ending point of tasks and zero for the depot.
    std::vector<int> demands(instance.get_num_agents() + 2 * instance.get_num_tasks() + 1, 0);
    for (int i = 0; i < instance.get_num_tasks(); ++i) {
        demands[instance.get_num_agents() + 2 * i] = 1;
        demands[instance.get_num_agents() + 2 * i + 1] = -1;
    }

    // =============== ROUTING OBJECTS =============================================================

    RoutingIndexManager manager{instance.get_num_agents() + instance.get_num_tasks() * 2
                                    + 1,  // the number of nodes: agents + tasks + depot
                                instance.get_num_agents(),  // the number of agents
                                starts,                     // starting nodes
                                ends};                      // ending nodes

    RoutingModel routing{manager};

    // =============== DISTANCE CALLBACK ===========================================================

    auto distance_callback = [&h_table, &manager, &depot, &instance, &node_to_ravel_point](
                                 int64_t from_index, int64_t to_index) -> int64_t {
        auto from_node = manager.IndexToNode(from_index);
        auto to_node = manager.IndexToNode(to_index);
        // the distance from all nodes to the depot is zero
        if (to_node == depot) {
            return 0;
        }
        auto from_point = instance.unravel(node_to_ravel_point.at(from_node));
        auto to_point = instance.unravel(node_to_ravel_point.at(to_node));
        return h_table.at(from_point).at(to_point);
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

    for (const auto& [start, end] : instance.get_tasks()) {
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
        throw std::runtime_error("OR-Tools can't find a solution.");
    }

    // =============== CONSTRUCTING THE GOAL SEQUENCES FROM THE SOLUTION ===========================

    std::vector<path_t> goal_sequences{};
    for (int agent = 0; agent < instance.get_num_agents(); ++agent) {
        auto index{routing.Start(agent)};
        path_t path{};
        while (!routing.IsEnd(index)) {
            path.emplace_back(instance.unravel(node_to_ravel_point.at(manager.IndexToNode(index))));
            index = solution->Value(routing.NextVar(index));
        }
        goal_sequences.push_back(path);
    }

    return goal_sequences;  // Ciccio Spastic
}

}  // namespace cmapd
