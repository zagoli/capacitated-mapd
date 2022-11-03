//
// Created by Jacopo on 02/11/2022.
//
#pragma once
#include <optional>
#include <utility>
#include <vector>

#include "Conflict.h"
#include "Constraint.h"
#include "ambient/AmbientMapInstance.h"
#include "custom_types.h"

namespace cmapd::cbs {

class Node {
  private:
    std::vector<path_t> m_paths;
    static std::optional<Conflict> detect_conflict(int first_agent,
                                                   int second_agent,
                                                   const path_t& first_path,
                                                   const path_t& second_path);

  public:
    explicit Node(const AmbientMapInstance& instance,
                  std::vector<path_t> goal_sequences,
                  const std::vector<Constraint>& constraints = {});
    std::vector<int> get_lengths();
    int get_makespan();
    std::optional<Conflict> get_first_conflict();
    [[nodiscard]] const std::vector<path_t>& get_paths() const;
};

}  // namespace cmapd::cbs
