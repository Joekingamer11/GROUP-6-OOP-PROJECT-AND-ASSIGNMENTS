/**
 * @file colormap.cpp
 * @brief Implementation of color manipulation, hex conversion, blending, palettes, and colormaps.
 *
 * Provides utilities for parsing hex color codes, interpolating RGB colors,
 * accessing standard categorical palettes (e.g., Seaborn deep), constructing
 * continuous gradient colormaps (e.g., viridis, magma, coolwarm), and computing
 * perceptual luminance for high-contrast text overlays.
 */

#include "intriviz/colormap.hpp"
#include "intriviz/error.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace intriviz {

// =========================================================================
// Week 1 Implementations
// =========================================================================

/**
 * @brief Parses a 6-digit hexadecimal color string into an RGB Color struct.
 * 
 * Supports both raw hex strings ("RRGGBB") and prefixed hex strings ("#RRGGBB").
 * 
 * @param hex The hexadecimal color string.
 * @return Decoded Color with r, g, b components.
 * @throws InvalidArgument If the hex string is malformed or contains non-hex characters.
 */
Color color_from_hex(const std::string& hex) {
    std::string s = hex;
    if (!s.empty() && s[0] == '#') {
        s = s.substr(1);
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

/**
 * @brief Formats an RGB Color into an uppercase 6-digit hex string with '#' prefix.
 * 
 * @param c The Color to format.
 * @return Hexadecimal representation (e.g., "#FF8800").
 */
std::string color_to_hex(Color c) {
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(c.r)
        << std::setw(2) << static_cast<int>(c.g)
        << std::setw(2) << static_cast<int>(c.b);
    return oss.str();
}

/**
 * @brief Linearly blends two RGB colors based on an interpolation factor t.
 * 
 * Clamps t to the range [0.0, 1.0].
 * 
 * @param a Start color (corresponds to t = 0.0).
 * @param b End color (corresponds to t = 1.0).
 * @param t Interpolation parameter.
 * @return Blended Color with rounded channel components.
 */
Color blend(Color a, Color b, double t) {
    double clamped_t = std::clamp(t, 0.0, 1.0);
    Color result;
    result.r = static_cast<std::uint8_t>(std::round(a.r + (b.r - a.r) * clamped_t));
    result.g = static_cast<std::uint8_t>(std::round(a.g + (b.g - a.g) * clamped_t));
    result.b = static_cast<std::uint8_t>(std::round(a.b + (b.b - a.b) * clamped_t));
    return result;
}

/**
 * @brief Retrieves a named categorical color palette.
 * 
 * Returns the 10-color Seaborn "deep" palette when name is "deep".
 * 
 * @param name The palette name identifier.
 * @return Vector of distinct categorical Color objects.
 * @throws InvalidArgument If the palette name is unknown.
 */
std::vector<Color> categorical_palette(const std::string& name) {
    if (name != "deep") {
        throw InvalidArgument("Unknown categorical palette name: " + name);
    }
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

/**
 * @brief Constructs a continuous Colormap from an ordered sequence of color stops.
 * 
 * @param stops Vector of at least 2 Color stops spanning from t = 0.0 to t = 1.0.
 * @throws InvalidArgument If fewer than 2 stops are provided.
 */
Colormap::Colormap(std::vector<Color> stops) : stops_(std::move(stops)) {
    if (stops_.size() < 2) {
        throw InvalidArgument("Colormap requires at least 2 color stops");
    }
}

/**
 * @brief Samples the colormap gradient at normalized position t in [0.0, 1.0].
 * 
 * Linearly interpolates between the adjacent color stops nearest to t.
 * Clamps t to [0.0, 1.0].
 * 
 * @param t Normalized parameter between 0.0 and 1.0.
 * @return Interpolated Color at position t.
 */
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

// =========================================================================
// Week 2 Implementations
// =========================================================================

/**
 * @brief Factory function creating standard built-in continuous colormaps by name.
 * 
 * Supported names: "viridis", "magma", "coolwarm", "gray" / "grey".
 * 
 * @param name The identifier of the desired colormap.
 * @return The constructed Colormap instance.
 * @throws InvalidArgument If an unrecognized colormap name is requested.
 */
Colormap make_colormap(const std::string& name) {
    if (name == "viridis") {
        return Colormap({
            color_from_hex("#440154"),
            color_from_hex("#3B528B"),
            color_from_hex("#21918C"),
            color_from_hex("#5EC962"),
            color_from_hex("#FDE725")
        });
    } else if (name == "magma") {
        return Colormap({
            color_from_hex("#000004"),
            color_from_hex("#51127C"),
            color_from_hex("#B73779"),
            color_from_hex("#FC8761"),
            color_from_hex("#FCFDBF")
        });
    } else if (name == "coolwarm") {
        return Colormap({
            color_from_hex("#3B4CC0"),
            color_from_hex("#DDDDDD"),
            color_from_hex("#B40426")
        });
    } else if (name == "gray" || name == "grey") {
        return Colormap({
            Color{0, 0, 0},
            Color{255, 255, 255}
        });
    }
    throw InvalidArgument("Unknown colormap name: " + name);
}

/**
 * @brief Safely indexes into a categorical palette with modulo wrap-around.
 * 
 * @param palette Vector of colors.
 * @param index Category or element index.
 * @return Color at index % palette.size().
 * @throws InvalidArgument If palette is empty.
 */
Color palette_color(const std::vector<Color>& palette, std::size_t index) {
    if (palette.empty()) {
        throw InvalidArgument("palette_color requires a non-empty palette");
    }
    return palette[index % palette.size()];
}

/**
 * @brief Calculates perceived photographic luminance (Y) according to ITU-R BT.601.
 * 
 * Formula: 0.299 * R + 0.587 * G + 0.114 * B.
 * 
 * @param c The color to evaluate.
 * @return Perceived luminance score between 0.0 and 255.0.
 */
double luminance(Color c) {
    return 0.299 * c.r + 0.587 * c.g + 0.114 * c.b;
}

/**
 * @brief Chooses high-contrast text color (white or black) for readability against a background.
 * 
 * @param background The underlying cell or canvas color.
 * @return White (255, 255, 255) for dark backgrounds (luminance < 140), Black (0, 0, 0) otherwise.
 */
Color contrast_text_color(Color background) {
    if (luminance(background) < 140.0) {
        return Color{255, 255, 255}; // White text for dark background
    }
    return Color{0, 0, 0}; // Black text for light background
}

} // namespace intriviz
