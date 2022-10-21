//
// Created by dade on 20/10/22.
//

#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "AmbientMap.h"
#include "Point.h"

namespace cmapd {

class AmbientMapInstance : public AmbientMap{
  private:
    std::vector<Point> m_agents;
    std::vector<std::pair<Point,Point>> m_tasks;
    
  public:
    explicit AmbientMapInstance(const std::filesystem::path& path_to_map,
                       const std::filesystem::path& path_to_map_instance);
    [[nodiscard]] int get_num_agents() const;
    [[nodiscard]] int get_num_tasks() const;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::vector<std::pair<Point,Point>> get_tasks() const;
    
    
};

}  // namespace cmapd

