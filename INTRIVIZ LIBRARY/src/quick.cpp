#include "cppviz/quick.hpp"
#include "cppviz/swarmplot.hpp"
#include "cppviz/pointplot.hpp"
#include "cppviz/heatmap.hpp"
#include "cppviz/stats.hpp"
#include "cppviz/labels.hpp"
#include "cppviz/legend.hpp"
#include "cppviz/validation.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"

namespace cppviz {

Figure quick_swarmplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title) {
    // Validate inputs
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    // Construct figure with standard dimensions (640x480)
    Figure fig(640, 480);
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

Figure quick_pointplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title) {
    // Validate inputs
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    // Construct figure with standard dimensions (640x480)
    Figure fig(640, 480);
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

    // Heatmaps with colorbars need a larger right margin (90px)
    Figure fig(640, 480);
    Margins m = {70, 90, 40, 60}; // left, right, top, bottom
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

} // namespace cppviz