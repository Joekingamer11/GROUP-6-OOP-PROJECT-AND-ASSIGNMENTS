#include "mini_test.hpp"
#include "cppviz/heatmap.hpp"
#include "cppviz/axes.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"
#include "cppviz/theme.hpp"
#include "cppviz/types.hpp"

#include <cmath>
#include <limits>
#include <string>
#include <vector>

int main() {
    using namespace cppviz;

    // 1. Normal 2x2 Heatmap Rendering and Color Sampling
    {
        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 40, 260, 200}, theme_white());

        std::vector<std::vector<double>> matrix = {
            {0.0, 1.0},
            {2.0, 3.0}
        };
        std::vector<std::string> row_labels = {"Row0", "Row1"};
        std::vector<std::string> col_labels = {"Col0", "Col1"};

        HeatmapOptions opts;
        opts.colormap = "viridis";
        opts.show_colorbar = false;

        heatmap(ax, matrix, row_labels, col_labels, opts);

        Colormap cmap = make_colormap("viridis");
        Color c_min = cmap.at(0.0);
        Color c_max = cmap.at(1.0);

        // Check top-left cell (row 0, col 0 = 0.0, min value -> c_min)
        PointI pt_min = ax.to_pixel(0.0, 0.0);
        CHECK(canvas.get_pixel(pt_min.x, pt_min.y) == c_min);

        // Check bottom-right cell (row 1, col 1 = 3.0, max value -> c_max)
        PointI pt_max = ax.to_pixel(1.0, 1.0);
        CHECK(canvas.get_pixel(pt_max.x, pt_max.y) == c_max);
    }