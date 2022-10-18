//
// Created by Jacopo on 18/10/2022.
//

#pragma once
#include <map>

#include "Point.h"

// TODO: replace with header import ---------------------
#include "../AmbientMap.h"
class AmbientMapInstance : public cmapd::AmbientMap {
  public:
    size_t get_num_agents();
    size_t get_num_tasks();
    std::vector<std::pair<Point, Point>> get_tasks() const;
};
// TODO: -------------------------------------------------

namespace cmapd {
using hTable_t = std::map<Point, std::map<Point, unsigned>>;

hTable_t compute_HTable(const AmbientMapInstance& map_instance, auto distance_function);
unsigned manhattan_distance(Point first, Point second);

}  // namespace cmapd