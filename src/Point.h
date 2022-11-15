/**
 * @file
 * @brief Contains the struct Point.
 * @author Jacopo Zagoli
 * @version 1.0
 * @date October, 2022
 * @copyright 2022 Jacopo Zagoli, Davide Furlani
 */

#pragma once

#include <compare>
#include <ostream>
#include <utility>

namespace cmapd {
/**
 * @struct Point
 * @brief Struct that represent a point in a Map
 * @param row The row number of the point
 * @param col The column number of the point
 */
struct Point {
    /// The rows index.
    int row;
    /// The column index.
    int col;
    /**
     * Constructs a Point with the specified values for row and column.
     * @param r The row number.
     * @param c The column number.
     */
    inline Point(int r, int c)
        : row{r},
          col{c} {}
    /// Comparisons are needed for standard library algorithms and data structures to work properly.
    [[nodiscard]] inline auto operator<=>(const Point&) const = default;
    /// Sum operation to generate surrounding Points. It's not a sum between Points
    /// because it would not make sense, it's only a way to generate an offsetted Point.
    Point& operator+=(const std::pair<int, int>& rhs);
    /// Used to print a Point to console.
    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

inline Point& Point::operator+=(const std::pair<int, int>& rhs) {
    row += rhs.first;
    col += rhs.second;
    return *this;
}

/// Used to print a Point to console.
inline std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "{" << point.row << ", " << point.col << "}";
    return os;
}

/// Sum operation to generate surrounding Points.
inline Point operator+(const Point& lhs, const std::pair<int, int>& rhs) {
    auto result{lhs};
    result += rhs;
    return result;
}

}  // namespace cmapd