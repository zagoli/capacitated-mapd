//
// Created by dade on 12/10/22.
//
#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "Point.h"

namespace cmapd {
class AmbientMap {
  private:
    std::vector<std::vector<char>> grid;

  public:
    explicit AmbientMap(std::filesystem::path path_to_map);
    [[nodiscard]] size_t get_rows() const;
    [[nodiscard]] size_t get_columns() const;
    [[nodiscard]] bool is_valid_position(Point p) const;
    [[nodiscard]] std::string to_string() const;
};

}  // namespace cmapd
