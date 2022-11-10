//
// Created by Jacopo on 10/11/2022.
//

#pragma once
#include <fmt/format.h>

#include "custom_types.h"

inline void are_valid_routes(const std::vector<cmapd::path_t>& routes) {
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

    for (size_t i = 0; i < max_path_len; ++i) {
        for (size_t path_to_check = 0; path_to_check < routes.size(); ++path_to_check) {
            if (i >= routes.at(path_to_check).size()) {
                continue;
            }
            for (size_t path_checked = path_to_check + 1; path_checked < routes.size();
                 ++path_checked) {
                if (i < routes.at(path_checked).size()) {
                    if (routes.at(path_to_check)[i] == routes.at(path_checked)[i]) {
                        throw std::runtime_error(
                            fmt::format("Conflict found between {} and {} at time {}",
                                        path_to_check,
                                        path_checked,
                                        i));
                    }
                }
            }
        }
    }
}
