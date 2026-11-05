/**
 * @file quick.cpp
 * @brief Implementation of one-line high-level convenience plotting APIs.
 *
 * Provides ready-to-save high-level visualization functions:
 * - quick_swarmplot(): Creates an 800x600 swarm plot with labels and legend.
 * - quick_pointplot(): Creates an 800x600 point plot with 95% bootstrap CI and labels.
 * - quick_heatmap(): Computes a correlation matrix and renders an annotated heatmap with colorbar.
 */

#include "intriviz/quick.hpp"
#include "intriviz/swarmplot.hpp"
#include "intriviz/pointplot.hpp"
#include "intriviz/heatmap.hpp"
#include "intriviz/stats.hpp"
#include "intriviz/labels.hpp"
#include "intriviz/legend.hpp"
#include "intriviz/validation.hpp"
#include "intriviz/groupby.hpp"
#include "intriviz/colormap.hpp"
#include "intriviz/error.hpp"

namespace intriviz {

/**
 * @brief High-level helper to generate a complete publication-ready swarm plot in a single call.
 * 
 * Creates an 800x600 Figure, colors observations by category using the "deep" palette,
 * adds title and axis labels, and embeds a categorical legend.
 * 
 * @param data Source Dataset.
 * @param x_column Categorical text column name.
 * @param y_column Continuous numeric column name.
 * @param title Optional chart title.
 * @return Fully assembled Figure instance ready to display or save.
 */
Figure quick_swarmplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title) {
    // Validate inputs
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    // Construct figure with dimensions giving room for labels at font_scale=2
    Figure fig(800, 600);
    Axes& ax = fig.axes();

    // Configure swarmplot options
    SwarmplotOptions opts;
    opts.color_by_category = true;

    // Render swarmplot onto axes
    swarmplot(ax, data, x_column, y_column, opts);

    // Add titles and axis labels
    if (!title.empty()) {
        draw_title(ax, title);
    }
    draw_xlabel(ax, x_column);
    draw_ylabel(ax, y_column);

    // Draw legend for category colors
    auto categories = unique_values(data.text(x_column));
    auto palette = categorical_palette("deep");
    std::vector<Color> colors;
    colors.reserve(categories.size());
    for (std::size_t i = 0; i < categories.size(); ++i) {
        colors.push_back(palette_color(palette, i));
    }
    draw_legend(ax, categories, colors, x_column);

    return fig;
}

/**
 * @brief High-level helper to generate a complete categorical point plot with confidence intervals.
 * 
 * Creates an 800x600 Figure, computes 95% bootstrap confidence intervals for category means,
 * connects means with lines, and attaches title and axis labels.
 * 
 * @param data Source Dataset.
 * @param x_column Categorical text column name.
 * @param y_column Continuous numeric column name.
 * @param title Optional chart title.
 * @return Fully assembled Figure instance ready to display or save.
 */
Figure quick_pointplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title) {
    // Validate inputs
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    // Construct figure with dimensions giving room for labels at font_scale=2
    Figure fig(800, 600);
    Axes& ax = fig.axes();

    // Configure pointplot options (95% CI enabled)
    PointplotOptions opts;
    opts.show_ci = true;

    // Render pointplot onto axes
    pointplot(ax, data, x_column, y_column, opts);

    // Add titles and axis labels
    if (!title.empty()) {
        draw_title(ax, title);
    }
    draw_xlabel(ax, x_column);
    draw_ylabel(ax, y_column);

    return fig;
}

/**
 * @brief High-level helper to compute a correlation matrix and render an annotated heatmap.
 * 
 * Creates an 800x600 Figure with extra right-margin space for a colorbar, computes
 * Pearson correlation coefficients across the specified numeric columns, renders
 * cells with the "viridis" colormap, and displays numerical value annotations rounded to 2 decimals.
 * 
 * @param data Source Dataset.
 * @param numeric_columns List of numeric column names to compute correlations across.
 * @param title Optional chart title.
 * @return Fully assembled Figure instance ready to display or save.
 * @throws DataError If numeric_columns is empty or dataset is empty.
 */
Figure quick_heatmap(const Dataset& data,
                      const std::vector<std::string>& numeric_columns,
                      const std::string& title) {
    // Validate inputs
    require_not_empty(data);
    if (numeric_columns.empty()) {
        throw DataError("quick_heatmap requires at least one numeric column");
    }
    for (const auto& col : numeric_columns) {
        require_numeric_column(data, col);
    }

    // Heatmaps with colorbars need a larger right margin
    Figure fig(800, 600);
    Margins m = {100, 110, 50, 80}; // left, right, top, bottom
    fig.set_margins(m);
    Axes& ax = fig.axes();

    // Compute correlation matrix
    auto matrix = correlation_matrix(data, numeric_columns);

    // Render heatmap with annotations enabled
    HeatmapOptions opts;
    opts.colormap = "viridis";
    opts.annotate = true;
    opts.decimals = 2;
    opts.show_colorbar = true;

    heatmap(ax, matrix, numeric_columns, numeric_columns, opts);

    // Add title if provided
    if (!title.empty()) {
        draw_title(ax, title);
    }

    return fig;
}

} // namespace intriviz
