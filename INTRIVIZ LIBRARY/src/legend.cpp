#include "cppviz/legend.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/error.hpp"
#include "cppviz/font.hpp"
#include <algorithm>

namespace cppviz {

void draw_legend(Axes& ax, 
                 const std::vector<std::string>& names,
                 const std::vector<Color>& colors, 
                 const std::string& title) {
    if (names.size() != colors.size()) {
        throw InvalidArgument("draw_legend: names and colors vectors must have equal size");
    }

    if (names.empty()) {
        return;
    }

    const auto& theme = ax.theme();
    Canvas& canvas = ax.canvas();
    const Rect& area = ax.area();

    int scale = std::max(1, theme.font_scale / 2);
    int line_h = text_height(scale);
    int pad = 6;
    int swatch_size = 10;
    int gap = 6;

    // Calculate maximum width required for entries and optional title
    int max_text_w = 0;
    if (!title.empty()) {
        max_text_w = text_width(title, scale);
    }
    for (const auto& name : names) {
        max_text_w = std::max(max_text_w, text_width(name, scale));
    }

    int box_w = pad + swatch_size + gap + max_text_w + pad;
    int entry_count = static_cast<int>(names.size());
    int content_rows = entry_count + (!title.empty() ? 1 : 0);
    int box_h = pad + content_rows * (line_h + 4) + pad;

    // Position legend box in top-right corner of the plot area
    int box_x = area.x + area.w - box_w - 8;
    int box_y = area.y + 8;

    // Keep box within plot area boundaries
    box_x = std::max(area.x, box_x);
    box_y = std::max(area.y, box_y);

    // Draw background and outline
    fill_rect(canvas, box_x, box_y, box_w, box_h, theme.plot_background);
    draw_rect(canvas, box_x, box_y, box_w, box_h, theme.axis_color);

    int cur_y = box_y + pad;

    // Render title if present
    if (!title.empty()) {
        draw_text(canvas, box_x + pad, cur_y, title, theme.text_color, scale);
        cur_y += line_h + 4;
    }

    // Render each category color swatch and label
    for (std::size_t i = 0; i < names.size(); ++i) {
        int swatch_x = box_x + pad;
        int swatch_y = cur_y + (line_h - swatch_size) / 2;

        fill_rect(canvas, swatch_x, swatch_y, swatch_size, swatch_size, colors[i]);
        draw_rect(canvas, swatch_x, swatch_y, swatch_size, swatch_size, theme.axis_color);

        int text_x = swatch_x + swatch_size + gap;
        draw_text(canvas, text_x, cur_y, names[i], theme.text_color, scale);

        cur_y += line_h + 4;
    }
}

}