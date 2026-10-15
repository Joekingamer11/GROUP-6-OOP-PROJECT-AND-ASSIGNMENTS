#pragma once

#include <string>
#include <vector>
#include "cppviz/types.hpp"

namespace cppviz {

/**
 * @brief Parses a 6-digit hexadecimal color string into a Color struct.
 * @param hex Color string in "#RRGGBB" or "RRGGBB" format.
 * @return Color struct containing R, G, B components (0..255).
 * @throws InvalidArgument if string is not exactly 6 hex digits.
 */
Color color_from_hex(const std::string& hex);

/**
 * @brief Formats a Color struct into a 6-digit uppercase hexadecimal string.
 * @param c Input Color struct.
 * @return Formatted string in "#RRGGBB" format.
 */
std::string color_to_hex(Color c);

/**
 * @brief Linearly interpolates between two colors.
 * @param a Start color (t = 0.0).
 * @param b End color (t = 1.0).
 * @param t Interpolation factor, clamped to [0.0, 1.0].
 * @return Interpolated Color.
 */
Color blend(Color a, Color b, double t);

/**
 * @brief Retrieves a categorical color palette by name.
 * @param name Palette name (defaults to "deep").
 * @return Vector of Colors representing the palette.
 * @throws InvalidArgument if the palette name is unknown.
 */
std::vector<Color> categorical_palette(const std::string& name = "deep");

/**
 * @brief Represents a continuous colormap interpolating across multiple color stops.
 */
class Colormap {
public:
    /**
     * @brief Constructs a Colormap with a given vector of color stops.
     * @param stops Vector of Colors (must contain at least 2 colors).
     * @throws InvalidArgument if fewer than 2 stops are provided.
     */
    explicit Colormap(std::vector<Color> stops);

    /**
     * @brief Samples a color from the gradient at position t.
     * @param t Position factor in [0.0, 1.0] (clamped if out of bounds).
     * @return Interpolated Color at position t.
     */
    Color at(double t) const;

private:
    std::vector<Color> stops_;
};

} // namespace cppviz
