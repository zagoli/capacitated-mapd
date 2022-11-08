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

    std::vector<int> demands(instance.get_num_agents() + 2 * instance.get_num_tasks() + 1,
                             0);  // lungo agenti_tast_task_agenti
    for (int i = 0; i < instance.get_num_tasks(); ++i) {
        demands[instance.get_num_agents() + 2 * i] = 1;
        demands[instance.get_num_agents() + 2 * i + 1] = -1;
    }

    // mappa i node (0,1,2,3...) al loro valore ravellato
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

    std::vector<RoutingIndexManager::NodeIndex> starts{};
    for (int i = 0; i < instance.get_num_agents(); ++i) {
        starts.emplace_back(i);
    }

    // unico punto di ritorno per tutti (specialissimo)
    const auto depot_value{instance.get_num_agents() + instance.get_num_tasks() * 2};
    const RoutingIndexManager::NodeIndex depot{static_cast<int>(depot_value)};
    std::vector<RoutingIndexManager::NodeIndex> ends{static_cast<size_t>(instance.get_num_agents()),
                                                     depot};

    RoutingIndexManager manager{instance.get_num_agents() + instance.get_num_tasks() * 2 + 1,
                                instance.get_num_agents(),
                                starts,
                                ends};

    RoutingModel routing{manager};

    // distance callback
    auto distance_callback = [&h_table, &manager, &depot, &instance, &node_to_ravel_point](
                                 int64_t from_index, int64_t to_index) -> int64_t {
        auto from_node = manager.IndexToNode(from_index);
        auto to_node = manager.IndexToNode(to_index);
        // hte distance from all nodes to the depot is zero
        if (to_node == depot) {
            return 0;
        }
        auto from_point = instance.unravel(node_to_ravel_point.at(from_node));
        auto to_point = instance.unravel(node_to_ravel_point.at(to_node));
        int64_t ret = h_table.at(from_point).at(to_point);
        return ret;
    };

    auto transit_callback_index{routing.RegisterTransitCallback(distance_callback)};
    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

    auto dimension_name{"Distance"};
    routing.AddDimension(
        transit_callback_index, 0, std::numeric_limits<int64_t>::max(), true, dimension_name);
    auto distance_dimension = routing.GetMutableDimension("Distance");
    distance_dimension->SetGlobalSpanCostCoefficient(100);

    auto demand_callback = [&demands, &manager](int64_t from_index) -> int64_t {
        int from_node = manager.IndexToNode(from_index).value();
        return demands.at(from_node);
    };
    const int demand_callback_index{routing.RegisterUnaryTransitCallback(demand_callback)};

    routing.AddDimension(demand_callback_index, int16_t(0), capacity, true, "Capacity");

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

    auto search_parameters{DefaultRoutingSearchParameters()};
    search_parameters.set_first_solution_strategy(
        FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION);

    const Assignment* solution{routing.SolveWithParameters(search_parameters)};

    if (solution == nullptr) {
        throw std::runtime_error("No solution found");
    }

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
