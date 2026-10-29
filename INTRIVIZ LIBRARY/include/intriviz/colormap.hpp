#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "intriviz/types.hpp"

namespace intriviz {

/**
 * @brief Parses a 6-digit hex string into a Color struct.
 * @param hex Hexadecimal string in format "#RRGGBB" or "RRGGBB".
 * @return Converted Color struct.
 * @throws InvalidArgument if length is not 6 or contains non-hex characters.
 */
Color color_from_hex(const std::string& hex);

/**
 * @brief Formats a Color struct into a 6-digit uppercase hex string.
 * @param c Input Color struct.
 * @return Formatted string in "#RRGGBB" format.
 */
std::string color_to_hex(Color c);

/**
 * @brief Linearly interpolates between two colors.
 * @param a Starting Color.
 * @param b Ending Color.
 * @param t Interpolation factor in range [0.0, 1.0].
 * @return Interpolated Color.
 */
Color blend(Color a, Color b, double t);

/**
 * @brief Retrieves a discrete categorical color palette.
 * @param name Name of the palette (default: "deep").
 * @return Vector of Color instances.
 * @throws InvalidArgument if palette name is unknown.
 */
std::vector<Color> categorical_palette(const std::string& name = "deep");

/**
 * @class Colormap
 * @brief Represents a continuous color gradient defined by multi-stop colors.
 */
class Colormap {
public:
    /**
     * @brief Constructs a Colormap with at least two color stops.
     * @param stops Vector of Color stops.
     * @throws InvalidArgument if fewer than 2 stops are provided.
     */
    explicit Colormap(std::vector<Color> stops);

    /**
     * @brief Samples a color along the gradient.
     * @param t Normalized position in range [0.0, 1.0].
     * @return Interpolated Color at position t.
     */
    Color at(double t) const;

private:
    std::vector<Color> stops_;
};

/**
 * @brief Factory function for creating predefined continuous colormaps.
 * @param name Colormap name ("viridis", "magma", "coolwarm", "gray").
 * @return Configured Colormap instance.
 * @throws InvalidArgument if the colormap name is unknown.
 */
Colormap make_colormap(const std::string& name);

/**
 * @brief Selects a color from a categorical palette using wrap-around indexing.
 * @param palette Vector of palette colors.
 * @param index Requested color index (uses index % palette.size()).
 * @return Selected Color from the palette.
 * @throws InvalidArgument if the palette is empty.
 */
Color palette_color(const std::vector<Color>& palette, std::size_t index);

/**
 * @brief Calculates the perceived brightness/luminance of a color.
 * @param c Input Color struct.
 * @return Perceived luminance value in range [0.0, 255.0].
 */
double luminance(Color c);

/**
 * @brief Determines readable text color (black or white) for a given background color.
 * @param background Background Color.
 * @return Black (Color{0,0,0}) or White (Color{255,255,255}).
 */
Color contrast_text_color(Color background);

} // namespace intriviz