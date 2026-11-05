/**
 * @file theme.cpp
 * @brief Implementation of built-in visualization themes.
 *
 * Provides factory functions for standard visual styles inspired by Seaborn,
 * configuring backgrounds, grid lines, axes colors, font sizing, and mark dimensions.
 */

#include "intriviz/theme.hpp"

namespace intriviz {

/**
 * @brief Creates a clean white theme with a white plot area and no background grid.
 * 
 * @return A configured Theme instance with white background and dark text/axes.
 */
Theme theme_white() {
    Theme t;
    t.background      = Color{255, 255, 255};
    t.plot_background = Color{255, 255, 255};
    t.axis_color      = Color{38, 38, 38};
    t.grid_color      = Color{220, 220, 220};
    t.text_color      = Color{38, 38, 38};
    t.show_grid       = false;
    t.font_scale      = 2;
    t.marker_radius   = 4;
    t.line_thickness  = 2;
    return t;
}

/**
 * @brief Creates a white theme with light grey grid lines enabled.
 * 
 * @return A Theme instance based on theme_white() with show_grid set to true.
 */
Theme theme_whitegrid() {
    Theme t = theme_white();
    t.show_grid  = true;
    t.grid_color = Color{220, 220, 220};
    return t;
}

/**
 * @brief Creates a theme with a light grey plot area and crisp white grid lines.
 * 
 * Matches Seaborn's default "darkgrid" aesthetic for clear data contrast.
 * 
 * @return A configured Theme instance with light grey plot fill and white grid.
 */
Theme theme_darkgrid() {
    Theme t;
    t.background      = Color{255, 255, 255};
    t.plot_background = Color{234, 234, 242}; // Seaborn signature light grey plot area
    t.axis_color      = Color{38, 38, 38};
    t.grid_color      = Color{255, 255, 255}; // Crisp white grid lines
    t.text_color      = Color{38, 38, 38};
    t.show_grid       = true;
    t.font_scale      = 2;
    t.marker_radius   = 4;
    t.line_thickness  = 2;
    return t;
}

/**
 * @brief Creates an inverted dark theme suitable for dark mode displays.
 * 
 * Features a dark grey canvas, muted dark grid, and light foreground text and axes.
 * 
 * @return A configured Theme instance with dark styling.
 */
Theme theme_dark() {
    Theme t;
    t.background      = Color{38, 38, 38};   // Dark background
    t.plot_background = Color{38, 38, 38};
    t.axis_color      = Color{220, 220, 220}; // Light axes
    t.grid_color      = Color{70, 70, 70};    // Muted dark grid
    t.text_color      = Color{220, 220, 220}; // Light text
    t.show_grid       = false;
    t.font_scale      = 2;
    t.marker_radius   = 4;
    t.line_thickness  = 2;
    return t;
}

} // namespace intriviz
