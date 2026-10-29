#include "mini_test.hpp"

#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"
#include "intriviz/pointplot.hpp"
#include "intriviz/theme.hpp"

int main() {
    using namespace intriviz;

    // Pointplot rendering and pixel color check
    {
        Dataset df;
        df.add_text_column("group", {"A", "A", "A", "B", "B", "B"});
        df.add_numeric_column("score", {10.0, 12.0, 14.0, 20.0, 22.0, 24.0});

        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts;
        opts.show_ci = true;
        opts.connect_points = true;

        pointplot(ax, df, "group", "score", opts);

        // To verify that drawings occurred on the canvas.
        const auto& pixels = canvas.data();
        bool has_non_white = false;
        for (std::size_t i = 0; i < pixels.size(); i += 3) {
            if (pixels[i] != 255 || pixels[i + 1] != 255 || pixels[i + 2] != 255) {
                has_non_white = true;
                break;
            }
        }
        CHECK(has_non_white);
    }

    // To verify mean pixel alignment.
    {
        Dataset df;
        df.add_text_column("group", {"A", "A", "B", "B"});
        df.add_numeric_column("value", {10.0, 10.0, 30.0, 30.0});

        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts;
        opts.show_ci = false;
        pointplot(ax, df, "group", "value", opts);

        PointI pt_a = ax.to_pixel(0.0, 10.0);
        PointI pt_b = ax.to_pixel(1.0, 30.0);

        CHECK(pt_a.y > pt_b.y);
    }

    // Option Flags
    {
        Dataset df;
        df.add_text_column("cat", {"C1", "C1", "C2", "C2"});
        df.add_numeric_column("val", {5.0, 15.0, 25.0, 35.0});

        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts;
        opts.show_ci = false;
        opts.connect_points = false;

        CHECK_NOTHROW(pointplot(ax, df, "cat", "val", opts));
    }

    // Checking for invalid inputs and error handling.
    {
        Dataset df;
        df.add_text_column("cat", {"A", "B"});
        df.add_numeric_column("val", {10.0, 20.0});

        Canvas canvas(400, 300);
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        CHECK_THROWS(pointplot(ax, df, "missing", "val"), DataError);
        CHECK_THROWS(pointplot(ax, df, "cat", "missing"), DataError);
        CHECK_THROWS(pointplot(ax, df, "val", "cat"), DataError);

        Dataset empty_df;
        CHECK_THROWS(pointplot(ax, empty_df, "cat", "val"), DataError);

        PointplotOptions bad_opts;
        bad_opts.ci_level = 1.5;
        CHECK_THROWS(pointplot(ax, df, "cat", "val", bad_opts), InvalidArgument);
    }

    return finish();
}
