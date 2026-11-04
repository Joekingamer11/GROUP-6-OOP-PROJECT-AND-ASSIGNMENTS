#include "intriviz/labels.hpp"
#include "intriviz/font.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

void draw_title(Axes& ax, const std::string& title) {
    if (title.empty()) {
        return;
    }

    const Rect& area = ax.area();
    int scale = ax.theme().font_scale;
    int tw = text_width(title, scale);

    // Shrink scale to 1 if text is wider than available plot area width
    if (tw > area.w && scale > 1) {
        scale = 1;
        tw = text_width(title, scale);
    }

    // Centred horizontally relative to the plot area
    int x = area.x + (area.w - tw) / 2;

    // Centred vertically within the top margin
    int th = text_height(scale);
    int y = (area.y > th) ? (area.y - th) / 2 : 0;

    draw_text(ax.canvas(), x, y, title, ax.theme().text_color, scale);
}

void draw_xlabel(Axes& ax, const std::string& label) {
    if (label.empty()) {
        return;
    }

    const Rect& area = ax.area();
    int scale = ax.theme().font_scale;
    int tw = text_width(label, scale);

    // Shrink scale to 1 if text is wider than available plot area width
    if (tw > area.w && scale > 1) {
        scale = 1;
        tw = text_width(label, scale);
    }

    // Centred horizontally relative to the plot area
    int x = area.x + (area.w - tw) / 2;

    // Placed below the bottom tick labels (tick label height + gap)
    int th = text_height(scale);
    int tick_label_bottom = area.y + area.h + 7 + text_height(ax.theme().font_scale);
    int y = tick_label_bottom + 8; // 8px gap below tick labels
    // Clamp so it does not run off canvas
    if (y + th > ax.canvas().height() - 2) {
        y = ax.canvas().height() - th - 2;
    }

    draw_text(ax.canvas(), x, y, label, ax.theme().text_color, scale);
}

void draw_ylabel(Axes& ax, const std::string& label) {
    if (label.empty()) {
        return;
    }

    const Rect& area = ax.area();
    int scale = ax.theme().font_scale;
    int th_vert = text_width(label, scale); // Vertical span when rotated

    // Shrink scale to 1 if vertical text exceeds plot area height
    if (th_vert > area.h && scale > 1) {
        scale = 1;
        th_vert = text_width(label, scale);
    }

    int tw_vert = text_height(scale); // Horizontal thickness when rotated

    // Centred vertically relative to the plot area
    int y = area.y + (area.h + th_vert) / 2;

    // Placed in the left margin, centered between canvas edge and tick labels
    int x = (area.x > tw_vert) ? (area.x - tw_vert) / 3 : 2;
    if (x < 2) x = 2;

    draw_text_vertical(ax.canvas(), x, y, label, ax.theme().text_color, scale);
}

} // namespace intriviz