#pragma once

#include "cppviz/types.hpp"

namespace cppviz {

/**
 * @brief Configuration structure defining the visual style and dimensions of plots.
 * 
 * Bundles background colors, axis stroke colors, grid colors, typography scale multipliers,
 * marker dimensions, and line stroke weights across all cppviz visual elements.
 */
struct Theme {
    Color background{255, 255, 255};       ///< Canvas margin/background color (default: white)
    Color plot_background{255, 255, 255};  ///< Plotting area interior background color
    Color axis_color{38, 38, 38};          ///< Axis line, tick mark, and spine color
    Color grid_color{220, 220, 220};        ///< Coordinate grid line color
    Color text_color{38, 38, 38};          ///< Title, tick label, and axis label text color
    bool  show_grid = false;               ///< Whether coordinate grid lines are rendered
    int   font_scale = 2;                  ///< Typography size scaling multiplier (>= 1)
    int   marker_radius = 4;             ///< Point and swarm plot marker dot radius (in pixels)
    int   line_thickness = 2;              ///< Plot line and axis stroke width (in pixels)
};

/**
 * @brief Constructs the "white" theme preset.
 * Plain white canvas and plot background with no grid lines.
 */
Theme theme_white();

/**
 * @brief Constructs the "whitegrid" theme preset.
 * White canvas and plot background with subtle light gray grid lines.
 */
Theme theme_whitegrid();

/**
 * @brief Constructs the "darkgrid" theme preset.
 * Light gray plot area with crisp white grid lines (Seaborn classic style).
 */
Theme theme_darkgrid();

/**
 * @brief Constructs the "dark" theme preset.
 * Dark gray canvas and plot background with high-contrast light text and axes.
 */
Theme theme_dark();

} // namespace cppviz