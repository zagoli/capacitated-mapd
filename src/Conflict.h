//
// Created by Jacopo on 02/11/2022.
//

#pragma once
#include "ConflictType.h"
#include "Point.h"

namespace cmapd {

struct Conflict {
    int first_agent;
    int second_agent;
    int timestep;
    Point first_position;
    Point second_position;
    ConflictType type;
    bool operator==(const Conflict& rhs) const = default;
};

}  // namespace cmapd
