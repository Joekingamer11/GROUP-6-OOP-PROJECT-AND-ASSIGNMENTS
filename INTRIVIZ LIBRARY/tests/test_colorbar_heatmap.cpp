#include "mini_test.hpp"
#include "intriviz/heatmap.hpp"
#include "intriviz/colorbar.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/colormap.hpp"
#include "intriviz/error.hpp"
#include "intriviz/figure.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

#include <cmath>
#include <vector>
#include <string>

int main() {
    using namespace intriviz;

    // 1. Heatmap with Integrated Colorbar Rendering Test
    {
        // Allocate canvas and figure with extra right margin for colorbar (e.g., right = 100)
        Figure fig(600, 400, theme_white());
        fig.set_margins(Margins{70, 100, 40, 60});

        std::vector<std::vector<double>> matrix = {
            {10.0, 20.0, 30.0},
            {40.0, 50.0, 60.0}
        };
        std::vector<std::string> row_labels = {"Row 1", "Row 2"};
        std::vector<std::string> col_labels = {"Col A", "Col B", "Col C"};

        HeatmapOptions opts;
        opts.colormap = "viridis";
        opts.annotate = true;
        opts.decimals = 1;
        opts.show_colorbar = true; // Renders colorbar adjacent to axes area

        heatmap(fig.axes(), matrix, row_labels, col_labels, opts);

        // Verify cell center pixels inside plot area match expected colormap values
        Colormap cmap = make_colormap("viridis");
        
        // Minimum cell (10.0 -> t = 0.0)
        PointI pt_min = fig.axes().to_pixel(0.0, 0.0);
        CHECK(fig.canvas().get_pixel(pt_min.x, pt_min.y) == cmap.at(0.0));

        // Maximum cell (60.0 -> t = 1.0)
        PointI pt_max = fig.axes().to_pixel(2.0, 1.0);
        CHECK(fig.canvas().get_pixel(pt_max.x, pt_max.y) == cmap.at(1.0));

        // Verify colorbar area (15px to the right of ax.area().x + ax.area().w)
        const Rect& area = fig.axes().area();
        int colorbar_x = area.x + area.w + 15;
        
        // Colorbar top row should correspond to t = 1.0 (vmax = 60.0)
        Color top_cb_pixel = fig.canvas().get_pixel(colorbar_x + 5, area.y);
        CHECK(top_cb_pixel == cmap.at(1.0));

        // Colorbar bottom row should correspond to t = 0.0 (vmin = 10.0)
        Color bottom_cb_pixel = fig.canvas().get_pixel(colorbar_x + 5, area.y + area.h - 1);
        CHECK(bottom_cb_pixel == cmap.at(0.0));
    }

    // 2. Custom vmin / vmax Bounds with Colorbar
    {
        Figure fig(500, 400, theme_white());
        fig.set_margins(Margins{60, 90, 40, 50});

        std::vector<std::vector<double>> matrix = {
            {0.0, 50.0},
            {100.0, 150.0}
        };
        std::vector<std::string> rows = {"R0", "R1"};
        std::vector<std::string> cols = {"C0", "C1"};

        HeatmapOptions opts;
        opts.colormap = "coolwarm";
        opts.vmin = -100.0; // custom lower bound
        opts.vmax = 200.0;  // custom upper bound
        opts.show_colorbar = true;

        heatmap(fig.axes(), matrix, rows, cols, opts);

        Colormap cmap = make_colormap("coolwarm");

        // Cell (0,0) = 0.0 -> t = (0 - (-100)) / (200 - (-100)) = 100/300 = 0.3333...
        PointI pt0 = fig.axes().to_pixel(0.0, 0.0);
        Color expected_c0 = cmap.at(100.0 / 300.0);
        CHECK(fig.canvas().get_pixel(pt0.x, pt0.y) == expected_c0);
    }

    // 3. Colorbar Standalone Integration on Axes
    {
        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 40, 200, 200}, theme_white());

        Colormap magma = make_colormap("magma");
        draw_colorbar(ax, magma, 0.0, 100.0);

        // Verify colorbar pixels exist at offset x = 50 + 200 + 15 = 265
        CHECK(canvas.get_pixel(265 + 5, 40) == magma.at(1.0));
        CHECK(canvas.get_pixel(265 + 5, 239) == magma.at(0.0));
    }

    // 4. Invalid Custom vmin >= vmax with Colorbar
    {
        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 40, 200, 200}, theme_white());

        Colormap viridis = make_colormap("viridis");
        // vmin >= vmax must throw InvalidArgument
        CHECK_THROWS(draw_colorbar(ax, viridis, 100.0, 50.0), InvalidArgument);
        CHECK_THROWS(draw_colorbar(ax, viridis, 50.0, 50.0), InvalidArgument);
    }

    return finish();
}
