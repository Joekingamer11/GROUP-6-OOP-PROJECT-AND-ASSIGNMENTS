#include "intriviz/theme.hpp"

namespace intriviz {

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

Theme theme_whitegrid() {
    Theme t = theme_white();
    t.show_grid  = true;
    t.grid_color = Color{220, 220, 220};
    return t;
}

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