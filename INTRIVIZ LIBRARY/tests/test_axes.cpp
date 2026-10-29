#include "mini_test.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/error.hpp"
#include "intriviz/scale.hpp"
#include <memory>

int main() {
    using namespace intriviz;

    Canvas canvas(800, 600, Color{255, 255, 255});
    Rect area{100, 50, 600, 400}; // Plot area: x=100..700, y=50..450
    Theme theme = theme_whitegrid();

    Axes ax(canvas, area, theme);

    // 1. Verify area and theme accessors
    CHECK_EQ(ax.area().x, 100);
    CHECK_EQ(ax.area().y, 50);
    CHECK_EQ(ax.area().w, 600);
    CHECK_EQ(ax.area().h, 400);

    // 2. Verify pixel ranges
    auto [x_left, x_right] = ax.x_pixel_range();
    CHECK_EQ(x_left, 100.0);
    CHECK_EQ(x_right, 700.0);

    auto [y_bottom, y_top] = ax.y_pixel_range();
    CHECK_EQ(y_bottom, 450.0); // Bottom is area.y + area.h
    CHECK_EQ(y_top, 50.0);     // Top is area.y

    // 3. Unset scales should throw InvalidArgument
    CHECK_THROWS(ax.x_scale(), InvalidArgument);
    CHECK_THROWS(ax.y_scale(), InvalidArgument);
    CHECK_THROWS(ax.to_pixel(0.0, 0.0), InvalidArgument);

    // 4. Set scales and verify mapping
    // X scale: data 0..10 mapped to pixels 100..700
    auto xs = std::make_shared<LinearScale>(0.0, 10.0, x_left, x_right);
    // Y scale: data 0..100 mapped to pixels 450..50 (reversed range)
    auto ys = std::make_shared<LinearScale>(0.0, 100.0, y_bottom, y_top);

    ax.set_x_scale(xs);
    ax.set_y_scale(ys);

    // Corners should land on area boundaries
    PointI p_bottom_left = ax.to_pixel(0.0, 0.0);
    CHECK_EQ(p_bottom_left.x, 100);
    CHECK_EQ(p_bottom_left.y, 450);

    PointI p_top_right = ax.to_pixel(10.0, 100.0);
    CHECK_EQ(p_top_right.x, 700);
    CHECK_EQ(p_top_right.y, 50);

    PointI p_center = ax.to_pixel(5.0, 50.0);
    CHECK_EQ(p_center.x, 400);
    CHECK_EQ(p_center.y, 250);

    // 5. Test draw_frame execution and pixel verification
    ax.draw_frame();

    // Check that the left axis line was drawn with axis_color
    Color left_edge = canvas.get_pixel(100, 250);
    CHECK(left_edge == theme.axis_color);

    // Check that plot background is filled inside area
    Color inside_bg = canvas.get_pixel(150, 150);
    // Should be plot background or grid color
    CHECK(inside_bg == theme.plot_background || inside_bg == theme.grid_color);

    return finish();
}