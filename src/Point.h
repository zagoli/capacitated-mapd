//
// Created by dade on 12/10/22.
//

#pragma once

#include <compare>
#include <utility>

namespace cmapd {
/**
 * @struct Point
 * @brief Struct that represent a point in a Map
 * @param row The row number of the point
 * @param col The column number of the point
 */
struct Point {
    int row;
    int col;
    // no empty Point allowed
    Point() = delete;
    inline Point(int r, int c)
        : row{r},
          col{c} {}
    // needed for h_table_t to work properly
    [[nodiscard]] inline auto operator<=>(const Point&) const = default;
    // sum operation to generate surrounding Points. It's not a sum between Points
    // because it would not make sense, it's only a way to generate an offsetted Point.
    Point& operator+=(const std::pair<int, int>& rhs);
};

inline Point& Point::operator+=(const std::pair<int, int>& rhs) {
    row += rhs.first;
    col += rhs.second;
    return *this;
}
inline Point operator+(const Point& lhs, const std::pair<int, int>& rhs) {
    auto result{lhs};
    result += rhs;
    return result;
}

}  // namespace cmapd