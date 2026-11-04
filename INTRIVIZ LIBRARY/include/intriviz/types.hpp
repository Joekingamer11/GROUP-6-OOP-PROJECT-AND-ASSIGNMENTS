#pragma once

#include <cstdint>
#include <ostream>

namespace intriviz {

struct Color {
    std::uint8_t r = 0; // 0..255
    std::uint8_t g = 0;
    std::uint8_t b = 0;

    constexpr Color() = default;
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : r(red), g(green), b(blue) {}
};

struct PointI {
    int x = 0; // pixel position
    int y = 0;
};

struct PointD {
    double x = 0.0; // data position
    double y = 0.0;
};

struct Rect {
    int x = 0; // top-left x
    int y = 0; // top-left y
    int w = 0; // width
    int h = 0; // height
};

struct Margins {
    int left   = 100; // space for y-axis label + tick labels at font_scale=2
    int right  = 30;  // breathing room on the right edge
    int top    = 50;  // room for title text
    int bottom = 80;  // room for x-axis tick labels + axis label
};

bool operator==(const Color& a, const Color& b);
bool operator!=(const Color& a, const Color& b);

inline std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "(" << static_cast<int>(color.r) << ", "
       << static_cast<int>(color.g) << ", "
       << static_cast<int>(color.b) << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const PointI& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const PointD& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Rect& rect) {
    os << "(" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Margins& margins) {
    os << "(" << margins.left << ", " << margins.right << ", "
       << margins.top << ", " << margins.bottom << ")";
    return os;
}

} // namespace intriviz