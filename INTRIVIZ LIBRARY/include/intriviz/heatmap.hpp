#pragma once

#include <string>
#include <vector>
#include "intriviz/axes.hpp"

namespace intriviz {

/**
 * @brief Configuration options for heatmap visualization.
 */
struct HeatmapOptions {
    std::string colormap = "viridis"; ///< Colormap name ("viridis", "magma", "coolwarm", "gray").
    bool annotate = false;            ///< If true, prints numeric value inside each cell.
    int decimals = 2;                 ///< Number of decimal places for cell value annotations.
    double vmin = 0.0;                ///< Minimum value for color scaling. If vmin == vmax, auto-computed.
    double vmax = 0.0;                ///< Maximum value for color scaling. If vmin == vmax, auto-computed.
    bool show_colorbar = true;        ///< If true, renders a vertical colorbar key beside the heatmap.
};

/**
 * @brief Renders a 2D matrix as a color-coded heatmap grid on the given Axes.
 * 
 * Matrix dimensions must match row_labels and col_labels sizes. Each cell (r, c)
 * is colored according to the normalized value mapped through the selected colormap.
 * 
 * @param ax Target Axes frame for drawing.
 * @param matrix 2D vector of data values [row][col].
 * @param row_labels Vector of category labels for matrix rows (Y-axis).
 * @param col_labels Vector of category labels for matrix columns (X-axis).
 * @param options Heatmap rendering options.
 * @throws DataError If matrix is empty or ragged (unequal row lengths).
 * @throws InvalidArgument If label vector sizes do not match matrix dimensions.
 */
void heatmap(Axes& ax, const std::vector<std::vector<double>>& matrix,
             const std::vector<std::string>& row_labels,
             const std::vector<std::string>& col_labels,
             const HeatmapOptions& options = {});

} // namespace intriviz
