//
// Created by Jacopo on 08/11/2022.
//

#pragma once

#include "CmapdSolution.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::cbs {

CmapdSolution cbs(const AmbientMapInstance& instance, const std::vector<path_t>& goal_sequences);

}
