//
// Created by dade on 04/11/22.
//

#pragma once
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd {

std::vector<path_t> assign_tasks(const AmbientMapInstance& instance,
                                 int capacity,
                                 const h_table_t& h_table);
}  // namespace cmapd