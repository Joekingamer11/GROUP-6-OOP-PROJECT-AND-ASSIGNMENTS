#pragma once

#include <string>

#include "intriviz/axes.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

/// \brief Formatting and calculation options for point plots.
struct PointplotOptions {
    Color  color{76, 114, 176};     ///< Mark and line color (default Seaborn blue #4C72B0)
    bool   show_ci = true;          ///< Whether to draw confidence interval error bars
    double ci_level = 0.95;         ///< Confidence level for bootstrap CI (e.g., 0.95 = 95%)
    int    marker_radius = 5;       ///< Radius of mean point markers in pixels
    int    line_thickness = 2;      ///< Line thickness for connecting lines and error bars
    bool   connect_points = true;   ///< Whether to draw connecting lines between category means
    int    cap_size = 4;            ///< Half-width of horizontal error bar caps in pixels
};

/// \brief Renders a point plot showing category means, confidence intervals, and connecting lines.
/// \param ax Target Axes object managing plot area and scales.
/// \param data Source Dataset containing data columns.
/// \param x_column Categorical text column name.
/// \param y_column Measurement numeric column name.
/// \param options Visual configurations for plot marks and confidence intervals.
/// \throws DataError if dataset is empty, or if columns are missing/invalid types.
/// \throws InvalidArgument if options parameters (e.g. ci_level) are out of bounds.
void pointplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const PointplotOptions& options = {});

} // namespace intriviz
