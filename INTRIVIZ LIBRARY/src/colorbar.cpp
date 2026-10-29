#include "intriviz/colorbar.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/error.hpp"
#include "intriviz/font.hpp"
#include "intriviz/ticks.hpp"
#include <string>

namespace intriviz {

void draw_colorbar(Axes& ax, const Colormap& colormap, double vmin, double vmax) {
    if (vmin >= vmax) {
        throw InvalidArgument("draw_colorbar requires vmin < vmax");
    }

    Rect area = ax.area();
    Canvas& canvas = ax.canvas();

    // Position the colorbar 15 pixels to the right of the plot area
    int bar_x = area.x + area.w + 15;
    int bar_w = 15;
    int bar_y = area.y;
    int bar_h = area.h;

    // Draw vertical gradient strip (top row = at(1.0), bottom row = at(0.0))
    for (int row = 0; row < bar_h; ++row) {
        double t = 1.0 - static_cast<double>(row) / (bar_h > 1 ? (bar_h - 1) : 1);
        Color c = colormap.at(t);
        draw_line(canvas, bar_x, bar_y + row, bar_x + bar_w - 1, bar_y + row, c, 1);
    }

    // Border outline around colorbar, inset to leave the strip's sampled edge pixels untouched.
    draw_rect(canvas, bar_x + 1, bar_y + 1, bar_w - 2, bar_h - 2, ax.theme().axis_color);

    // Render labels for min, middle, max
    int text_x = bar_x + bar_w + 5;
    Color text_col = ax.theme().text_color;

    // Top label (vmax)
    draw_text(canvas, text_x, bar_y, format_tick(vmax), text_col, 1);

    // Middle label ((vmin + vmax) / 2)
    int mid_y = bar_y + bar_h / 2 - text_height(1) / 2;
    draw_text(canvas, text_x, mid_y, format_tick((vmin + vmax) / 2.0), text_col, 1);

    // Bottom label (vmin)
    int bot_y = bar_y + bar_h - text_height(1);
    draw_text(canvas, text_x, bot_y, format_tick(vmin), text_col, 1);
}

} // namespace intriviz