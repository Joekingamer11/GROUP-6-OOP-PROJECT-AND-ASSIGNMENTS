#include "intriviz/heatmap.hpp"
#include "intriviz/categorical_scale.hpp"
#include "intriviz/colorbar.hpp"
#include "intriviz/colormap.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/error.hpp"
#include "intriviz/font.hpp"
#include "intriviz/validation.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>

namespace intriviz {

void heatmap(Axes& ax, const std::vector<std::vector<double>>& matrix,
             const std::vector<std::string>& row_labels,
             const std::vector<std::string>& col_labels,
             const HeatmapOptions& options) {
    // 1. Validate matrix structure
    if (matrix.empty()) {
        throw DataError("Heatmap matrix cannot be empty");
    }

    std::size_t num_rows = matrix.size();
    std::size_t num_cols = matrix.empty() ? 0 : matrix[0].size();

    if (num_rows == 0 || num_cols == 0) {
        throw DataError("Heatmap matrix rows cannot be empty");
    }

    for (std::size_t r = 0; r < num_rows; ++r) {
        if (matrix[r].size() != num_cols) {
            throw DataError(
                "Heatmap matrix is ragged: row " + std::to_string(r) + " has length " +
                std::to_string(matrix[r].size()) + ", expected " + std::to_string(num_cols)
            );
        }
    }

    // 2. Validate row and column label counts
    if (row_labels.size() != num_rows) {
        throw InvalidArgument(
            "Row labels size (" + std::to_string(row_labels.size()) +
            ") does not match matrix row count (" + std::to_string(num_rows) + ")"
        );
    }
    if (col_labels.size() != num_cols) {
        throw InvalidArgument(
            "Column labels size (" + std::to_string(col_labels.size()) +
            ") does not match matrix column count (" + std::to_string(num_cols) + ")"
        );
    }

    // 3. Determine value range (vmin, vmax)
    double vmin = options.vmin;
    double vmax = options.vmax;

    if (vmin == vmax) {
        bool first = true;
        for (std::size_t r = 0; r < num_rows; ++r) {
            for (std::size_t c = 0; c < num_cols; ++c) {
                double val = matrix[r][c];
                if (!std::isnan(val)) {
                    if (first) {
                        vmin = val;
                        vmax = val;
                        first = false;
                    } else {
                        vmin = std::min(vmin, val);
                        vmax = std::max(vmax, val);
                    }
                }
            }
        }
        if (first) {
            // All values were NaN
            vmin = 0.0;
            vmax = 1.0;
        } else if (vmin == vmax) {
            // Constant matrix: widen range to avoid division by zero
            vmin -= 0.5;
            vmax += 0.5;
        }
    }

    // 4. Configure categorical scales
    auto x_range = ax.x_pixel_range(); // {left, right}
    auto y_range = ax.y_pixel_range(); // {bottom, top}

    // Note: Y categorical scale maps top to bottom so row 0 appears at top of plot area
    auto x_scale = std::make_shared<CategoricalScale>(col_labels, x_range.first, x_range.second);
    auto y_scale = std::make_shared<CategoricalScale>(row_labels, y_range.second, y_range.first);

    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);

    // 5. Draw axes frame (background, grid, tick marks, tick labels)
    ax.draw_frame();

    // 6. Retrieve colormap
    Colormap cmap = make_colormap(options.colormap);

    // 7. Render matrix cells
    int cell_w = static_cast<int>(std::round(x_scale->band_width()));
    int cell_h = static_cast<int>(std::round(y_scale->band_width()));

    for (std::size_t r = 0; r < num_rows; ++r) {
        for (std::size_t c = 0; c < num_cols; ++c) {
            PointI center = ax.to_pixel(static_cast<double>(c), static_cast<double>(r));
            int cell_x = center.x - cell_w / 2;
            int cell_y = center.y - cell_h / 2;

            double val = matrix[r][c];
            Color cell_color;

            if (std::isnan(val)) {
                // NaN cells drawn in neutral light grey
                cell_color = Color{200, 200, 200};
            } else {
                double range = vmax - vmin;
                double t = (range != 0.0) ? (val - vmin) / range : 0.5;
                t = std::clamp(t, 0.0, 1.0);
                cell_color = cmap.at(t);
            }

            fill_rect(ax.canvas(), cell_x, cell_y, cell_w, cell_h, cell_color);

            // 8. Optional cell text annotations
            if (options.annotate && !std::isnan(val)) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(options.decimals) << val;
                std::string text = oss.str();

                Color text_col = contrast_text_color(cell_color);
                int tw = text_width(text, 1);
                int th = text_height(1);

                int text_x = center.x - tw / 2;
                int text_y = center.y - th / 2;

                // Draw text if it fits inside cell boundaries
                if (tw <= cell_w && th <= cell_h) {
                    draw_text(ax.canvas(), text_x, text_y, text, text_col, 1);
                }
            }
        }
    }

    // 9. Draw vertical colorbar if requested
    if (options.show_colorbar) {
        draw_colorbar(ax, cmap, vmin, vmax);
    }
}

} // namespace intriviz
