//
// Created by dade on 30/10/22.
//

#pragma once
#include <ambient/AmbientMap.h>
#include <ambient/AmbientMapInstance.h>

namespace cmapd {
std::vector<AmbientMapInstance> generate_instances(const AmbientMap& map,
                                                   std::filesystem::path save_path,
                                                   int n_instances,
                                                   int n_agents,
                                                   int n_tasks);
}
