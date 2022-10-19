//
// Created by dade on 12/10/22.
//

#pragma once
namespace cmapd {
struct Point {
    int row;
    int col;
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