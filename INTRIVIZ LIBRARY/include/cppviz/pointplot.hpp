#pragma once

#include <string>
#include "cppviz/axes.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/types.hpp"

namespace cppviz {

// Configuration options for point plots
struct PointplotOptions {
    Color  color{76, 114, 176};     // Default Seaborn blue (#4C72B0)
    bool   show_ci = true;          // Whether to draw confidence interval error bars
    double ci_level = 0.95;         // Confidence level for bootstrap CI (e.g. 0.95 = 95%)
    int    marker_radius = 5;       // Radius of category mean markers in pixels
    int    line_thickness = 2;      // Thickness of connecting lines and CI bars
    bool   connect_points = true;   // Whether to connect category means with a line
    int    cap_size = 4;            // Half-width of error bar end-caps in pixels
};

// Renders a point plot showing category means, confidence intervals, and connecting lines.
// x_column must be a text/categorical column; y_column must be numeric.
// Throws DataError or InvalidArgument on invalid inputs or missing columns.
void pointplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const PointplotOptions& options = {});

} // namespace cppviz