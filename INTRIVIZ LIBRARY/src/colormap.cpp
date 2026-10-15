#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace cppviz {

Color color_from_hex(const std::string& hex) {
    std::string s = hex;
    if (!s.empty() && s.front() == '#') {
        s.erase(0, 1);
    }
    if (s.length() != 6) {
        throw InvalidArgument("Hex color string must be exactly 6 hex digits, got: " + hex);
    }
    for (char ch : s) {
        if (!std::isxdigit(static_cast<unsigned char>(ch))) {
            throw InvalidArgument("Invalid hex character in string: " + hex);
        }
    }
    unsigned long val = std::stoul(s, nullptr, 16);
    Color c;
    c.r = static_cast<std::uint8_t>((val >> 16) & 0xFF);
    c.g = static_cast<std::uint8_t>((val >> 8) & 0xFF);
    c.b = static_cast<std::uint8_t>(val & 0xFF);
    return c;
}

std::string color_to_hex(Color c) {
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(c.r)
        << std::setw(2) << static_cast<int>(c.g)
        << std::setw(2) << static_cast<int>(c.b);
    return oss.str();
}

Color blend(Color a, Color b, double t) {
    double clamped_t = std::clamp(t, 0.0, 1.0);
    Color result;
    result.r = static_cast<std::uint8_t>(std::round(a.r + (b.r - a.r) * clamped_t));
    result.g = static_cast<std::uint8_t>(std::round(a.g + (b.g - a.g) * clamped_t));
    result.b = static_cast<std::uint8_t>(std::round(a.b + (b.b - a.b) * clamped_t));
    return result;
}

std::vector<Color> categorical_palette(const std::string& name) {
    if (name != "deep") {
        throw InvalidArgument("Unknown categorical palette name: " + name);
    }
    // Seaborn "deep" palette hex codes
    static const std::vector<std::string> deep_hexes = {
        "#4C72B0", "#DD8452", "#55A868", "#C44E52", "#8172B3",
        "#937860", "#DA8BC3", "#8C8C8C", "#CCB974", "#64B5CD"
    };
    std::vector<Color> palette;
    palette.reserve(deep_hexes.size());
    for (const auto& hex : deep_hexes) {
        palette.push_back(color_from_hex(hex));
    }
    return palette;
}

Colormap::Colormap(std::vector<Color> stops) : stops_(std::move(stops)) {
    if (stops_.size() < 2) {
        throw InvalidArgument("Colormap requires at least 2 color stops");
    }
}

Color Colormap::at(double t) const {
    double clamped_t = std::clamp(t, 0.0, 1.0);
    double pos = clamped_t * (stops_.size() - 1);
    std::size_t index = static_cast<std::size_t>(std::floor(pos));
    if (index >= stops_.size() - 1) {
        return stops_.back();
    }
    double frac = pos - index;
    return blend(stops_[index], stops_[index + 1], frac);
}

} // namespace cppviz