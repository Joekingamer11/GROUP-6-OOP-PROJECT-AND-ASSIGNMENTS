#include "mini_test.hpp"
#include "cppviz/legend.hpp"
#include "cppviz/axes.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/error.hpp"
#include "cppviz/theme.hpp"

int main() {
    using namespace cppviz;

    Canvas canvas(400, 300, Color{255, 255, 255});
    Rect area{50, 40, 300, 200};
    Theme theme = theme_white();
    Axes ax(canvas, area, theme);

    // 1. Validation: Mismatched lengths must throw InvalidArgument
    {
        bool threw = false;
        try {
            draw_legend(ax, {"A", "B"}, {Color{255, 0, 0}});
        } catch (const InvalidArgument&) {
            threw = true;
        } catch (...) {}
        CHECK(threw);
    }

    // 2. Empty input: Should leave canvas unchanged without throwing
    Canvas canvas_before = canvas;
    draw_legend(ax, {}, {});
    CHECK(canvas.data() == canvas_before.data());

    // 3. Normal rendering: 3 categories
    std::vector<std::string> names = {"Cat A", "Cat B", "Cat C"};
    std::vector<Color> colors = {
        Color{255, 0, 0},   // Red
        Color{0, 255, 0},   // Green
        Color{0, 0, 255}    // Blue
    };

    draw_legend(ax, names, colors, "Categories");

    // Verify canvas pixels were modified inside the plot area
    bool found_red = false;
    bool found_green = false;
    bool found_blue = false;

    for (int y = area.y; y < area.y + area.h; ++y) {
        for (int x = area.x; x < area.x + area.w; ++x) {
            Color p = canvas.get_pixel(x, y);
            if (p == Color{255, 0, 0}) found_red = true;
            if (p == Color{0, 255, 0}) found_green = true;
            if (p == Color{0, 0, 255}) found_blue = true;
        }
    }

    CHECK(found_red);
    CHECK(found_green);
    CHECK(found_blue);

    return finish();
}