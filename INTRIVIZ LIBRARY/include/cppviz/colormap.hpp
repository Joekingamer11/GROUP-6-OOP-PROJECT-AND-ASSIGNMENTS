#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "cppviz/types.hpp"

namespace cppviz {

// =========================================================================
// Week 1 Declarations
// =========================================================================

Color color_from_hex(const std::string& hex);
std::string color_to_hex(Color c);
Color blend(Color a, Color b, double t);
std::vector<Color> categorical_palette(const std::string& name = "deep");

class Colormap {
public:
    explicit Colormap(std::vector<Color> stops);
    Color at(double t) const;

private:
    std::vector<Color> stops_;
};

// =========================================================================
// Week 2 Declarations
// =========================================================================

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

} // namespace cppviz