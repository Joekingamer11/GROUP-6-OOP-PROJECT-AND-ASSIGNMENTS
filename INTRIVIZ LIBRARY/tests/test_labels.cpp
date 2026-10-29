#include "mini_test.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/labels.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

#include <string>

int main() {
    using namespace intriviz;

    // 1. Setup Canvas, Theme, and Axes
    Canvas canvas(400, 300, Color{255, 255, 255});
    Theme theme = theme_white();
    Rect area{70, 40, 310, 200};
    Axes ax(canvas, area, theme);

    // 2. Empty labels draw no pixels (canvas remains blank)
    draw_title(ax, "");
    draw_xlabel(ax, "");
    draw_ylabel(ax, "");

    bool remains_white = true;
    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            if (canvas.get_pixel(x, y) != Color{255, 255, 255}) {
                remains_white = false;
                break;
            }
        }
    }
    CHECK(remains_white);

    // 3. Test draw_title pixel placement in top margin
    draw_title(ax, "Total Bill");
    bool pixel_changed_top = false;
    for (int y = 0; y < area.y; ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            if (canvas.get_pixel(x, y) != Color{255, 255, 255}) {
                pixel_changed_top = true;
                break;
            }
        }
    }
    CHECK(pixel_changed_top);

    // 4. Test draw_xlabel pixel placement below plot area
    draw_xlabel(ax, "Day of Week");
    bool pixel_changed_bottom = false;
    for (int y = area.y + area.h; y < canvas.height(); ++y) {
        for (int x = 0; x < canvas.width(); ++x) {
            if (canvas.get_pixel(x, y) != Color{255, 255, 255}) {
                pixel_changed_bottom = true;
                break;
            }
        }
    }
    CHECK(pixel_changed_bottom);

    // 5. Test draw_ylabel pixel placement in left margin
    draw_ylabel(ax, "Amount ($)");
    bool pixel_changed_left = false;
    for (int y = 0; y < canvas.height(); ++y) {
        for (int x = 0; x < area.x; ++x) {
            if (canvas.get_pixel(x, y) != Color{255, 255, 255}) {
                pixel_changed_left = true;
                break;
            }
        }
    }
    CHECK(pixel_changed_left);

    // 6. Test long title (scale fallback to 1 without crashing)
    std::string long_title(100, 'A');
    draw_title(ax, long_title);

    return finish();
}