#include "mini_test.hpp"
#include "cppviz/axes.hpp"
#include "cppviz/colorbar.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"
#include "cppviz/theme.hpp"

int main() {
    using namespace cppviz;

    // 1. Invalid arguments (vmin >= vmax throws InvalidArgument)
    Canvas canvas(400, 300);
    Theme theme = theme_white();
    Axes ax(canvas, Rect{50, 40, 200, 200}, theme);
    Colormap cm = make_colormap("viridis");

    CHECK_THROWS(draw_colorbar(ax, cm, 10.0, 10.0), InvalidArgument);
    CHECK_THROWS(draw_colorbar(ax, cm, 20.0, 10.0), InvalidArgument);

    // 2. Normal rendering test
    draw_colorbar(ax, cm, 0.0, 100.0);

    // Top pixel of gradient strip (bar_x = 50 + 200 + 15 = 265, y = 40)
    Color top_color = canvas.get_pixel(265, 40);
    CHECK_EQ(top_color, cm.at(1.0));

    // Bottom pixel of gradient strip (y = 40 + 200 - 1 = 239)
    Color bot_color = canvas.get_pixel(265, 239);
    CHECK_EQ(bot_color, cm.at(0.0));

    return finish();
}