//
// Created by dade on 12/10/22.
//

#pragma once

struct Point {
    int row;
    int col;
    // needed for h_table_t to work properly
    [[nodiscard]] inline auto operator<=>(const Point&) const = default;
};