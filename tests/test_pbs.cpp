//
// Created by dade on 08/11/22.
//
#include <fmt/format.h>

#include <catch2/catch_test_macros.hpp>

#include "distances/distances.h"
#include "ortools/ortools.h"
#include "pbs.h"

namespace pbs_test {

using namespace cmapd;

void are_valid_routes(const std::vector<path_t>& routes) {
    if (routes.empty()) {
        throw std::runtime_error(fmt::format("boh, percorsi vuoti"));
    }
    if (routes.size() == 1) {
        return;
    }

    size_t max_path_len{0};

    for (const auto& route : routes) {
        if (route.size() > max_path_len) max_path_len = route.size();
    }

    for (size_t i = 0; i < max_path_len; ++i) {  // SCORRO GLI INDICI DA CONFRONTARE
        for (size_t path_to_check = 0; path_to_check < routes.size();
             ++path_to_check) {                          // SCORRO I PATH
            if (i >= routes.at(path_to_check).size()) {  // se l'indice che voglio confrontare
                                                         // esiste nel path che sto controllando
                continue;
            }
            for (size_t path_checked = path_to_check + 1; path_checked < routes.size();
                 ++path_checked) {  // E CONFRONTO CON GLI ALTRI PATH
                if (i < routes.at(path_checked)
                            .size()) {  // controllo che l'indice del punto da confrontare esista
                    if (routes.at(path_to_check)[i]
                        == routes.at(path_checked)[i]) {  // confronto i punti
                        throw std::runtime_error(
                            fmt::format("trovato conflitto tra {} e {} al tempo {}",
                                        path_to_check,
                                        path_checked,
                                        i));
                    }
                }
            }
        }
    }
}

TEST_CASE("pbs", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_2.txt", "data/map_2.txt"};
    const h_table_t h_table{compute_h_table(instance, manhattan_distance)};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 1, h_table);

    std::vector<path_t> final_temp_paths = pbs(instance, goal_sequences, h_table);
}

TEST_CASE("pbs_2", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_3.txt", "data/map_3.txt"};
    const h_table_t h_table{compute_h_table(instance, manhattan_distance)};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 2, h_table);

    std::vector<path_t> final_temp_paths = pbs(instance, goal_sequences, h_table);
}

TEST_CASE("pbs_4", "[pbs]") {
    const AmbientMapInstance instance{"data/instance_4.txt", "data/map_4.txt"};
    const h_table_t h_table{compute_h_table(instance, manhattan_distance)};
    std::vector<path_t> goal_sequences = assign_tasks(instance, 1, h_table);

    std::vector<path_t> final_temp_paths = pbs(instance, goal_sequences, h_table);
    REQUIRE_NOTHROW(are_valid_routes(final_temp_paths));
}
}  // namespace pbs_test