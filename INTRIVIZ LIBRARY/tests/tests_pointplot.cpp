#include "mini_test.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"
#include "intriviz/pointplot.hpp"
#include "intriviz/theme.hpp"

int main() {
    using namespace intriviz;

    // --- Prepare Sample Dataset ---
    Dataset ds;
    ds.add_text_column("group", {"A", "A", "A", "B", "B", "B"});
    ds.add_numeric_column("score", {10.0, 12.0, 14.0, 20.0, 22.0, 24.0});

    Canvas canvas(400, 300, Color{255, 255, 255});
    Rect area{50, 30, 300, 220};
    Axes ax(canvas, area, theme_whitegrid());

    // --- Normal Pointplot Test ---
    PointplotOptions opts;
    opts.color = Color{0, 0, 255}; // Blue
    opts.show_ci = true;
    opts.connect_points = true;

    pointplot(ax, ds, "group", "score", opts);

    // Verify canvas is modified (non-white pixels exist in plot area)
    bool found_plot_pixel = false;
    for (int y = area.y; y < area.y + area.h; ++y) {
        for (int x = area.x; x < area.x + area.w; ++x) {
            Color c = canvas.get_pixel(x, y);
            if (c != Color{255, 255, 255}) {
                found_plot_pixel = true;
                break;
            }
        }
        if (found_plot_pixel) break;
    }
    CHECK(found_plot_pixel);

    // --- Test Without Confidence Intervals and Connecting Lines ---
    Canvas canvas2(400, 300);
    Axes ax2(canvas2, area, theme_white());
    PointplotOptions opts_no_ci;
    opts_no_ci.show_ci = false;
    opts_no_ci.connect_points = false;

    pointplot(ax2, ds, "group", "score", opts_no_ci);

    // --- Invalid Inputs & Exceptions ---
    // Non-existent column
    CHECK_THROWS(pointplot(ax, ds, "missing_col", "score"), DataError);
    CHECK_THROWS(pointplot(ax, ds, "group", "missing_col"), DataError);

    // Wrong column types (numeric passed for x, text passed for y)
    CHECK_THROWS(pointplot(ax, ds, "score", "group"), DataError);

    // Empty dataset
    Dataset empty_ds;
    CHECK_THROWS(pointplot(ax, empty_ds, "group", "score"), DataError);

    // Invalid confidence level
    PointplotOptions bad_opts;
    bad_opts.ci_level = 1.5;
    CHECK_THROWS(pointplot(ax, ds, "group", "score", bad_opts), InvalidArgument);

    return finish();
}