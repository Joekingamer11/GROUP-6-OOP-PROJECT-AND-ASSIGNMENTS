#include "mini_test.hpp"
#include "intriviz/annotation.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/scale.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/error.hpp"

#include <memory>

int main() {
    using namespace intriviz;

    // 1. Calling annotations before scales are set throws InvalidArgument
    {
        Canvas canvas(400, 300);
        Theme theme = theme_white();
        Rect area{50, 40, 300, 200};
        Axes ax(canvas, area, theme);

        CHECK_THROWS(annotate_hline(ax, 5.0), InvalidArgument);
        CHECK_THROWS(annotate_vline(ax, 2.0), InvalidArgument);
        CHECK_THROWS(annotate_text(ax, 1.0, 1.0, "note"), InvalidArgument);
        CHECK_THROWS(annotate_arrow(ax, 0.0, 0.0, 1.0, 1.0, "arrow"), InvalidArgument);
    }

    // 2. Setup Axes with scales for rendering verification
    Canvas canvas(400, 300, Color{255, 255, 255});
    Theme theme = theme_white();
    Rect area{50, 40, 300, 200}; // x: 50..349, y: 40..239
    Axes ax(canvas, area, theme);

    auto x_scale = std::make_shared<LinearScale>(0.0, 10.0, 50.0, 350.0);
    auto y_scale = std::make_shared<LinearScale>(0.0, 10.0, 240.0, 40.0); // y reversed
    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);

    // 3. Horizontal reference line at y = 5.0 (pixel y = 140)
    {
        Color red{255, 0, 0};
        annotate_hline(ax, 5.0, red, 1);

        PointI p = ax.to_pixel(5.0, 5.0);
        CHECK_EQ(p.y, 140);
        CHECK(canvas.get_pixel(100, 140) == red);
        CHECK(canvas.get_pixel(200, 140) == red);
    }

    // 4. Vertical reference line at x = 5.0 (pixel x = 200)
    {
        Color blue{0, 0, 255};
        annotate_vline(ax, 5.0, blue, 1);

        PointI p = ax.to_pixel(5.0, 5.0);
        CHECK_EQ(p.x, 200);
        CHECK(canvas.get_pixel(200, 100) == blue);
        CHECK(canvas.get_pixel(200, 180) == blue);
    }

    // 5. Out-of-bounds lines are clipped without crashing
    {
        annotate_hline(ax, 50.0, Color{0, 255, 0});
        annotate_vline(ax, -20.0, Color{0, 255, 0});
    }

    // 6. Arrow annotation rendering
    {
        annotate_arrow(ax, 1.0, 1.0, 8.0, 8.0, "Peak");
        PointI p_to = ax.to_pixel(8.0, 8.0);
        CHECK(canvas.get_pixel(p_to.x, p_to.y) == theme.text_color);
    }

    return finish();
}