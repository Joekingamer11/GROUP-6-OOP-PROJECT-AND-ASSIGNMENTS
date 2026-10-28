#include "cppviz/pointplot.hpp"
#include "cppviz/categorical_scale.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/error.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/scale.hpp"
#include "cppviz/stats.hpp"
#include "cppviz/ticks.hpp"
#include "cppviz/validation.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace cppviz {

void pointplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const PointplotOptions& options) {
    
    // Input Validation
    
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    if (options.ci_level <= 0.0 || options.ci_level >= 1.0) {
        throw InvalidArgument("ci_level must be strictly between 0.0 and 1.0");
    }

    
    // Group Data and Compute Statistics
    
    std::vector<Group> groups = group_by(data, x_column, y_column);
    if (groups.empty()) {
        throw DataError("pointplot dataset has no valid observations");
    }

    std::vector<std::string> cat_names;
    std::vector<double> means;
    std::vector<Interval> cis;
    std::vector<double> all_y_values; // Used to compute padded y-axis domain

    cat_names.reserve(groups.size());
    means.reserve(groups.size());
    cis.reserve(groups.size());

    for (const auto& group : groups) {
        if (group.values.empty()) {
            continue;
        }

        cat_names.push_back(group.key);

        double m = mean(group.values);
        means.push_back(m);
        all_y_values.push_back(m);

        if (options.show_ci) {
            Interval ci = bootstrap_ci(group.values, options.ci_level, 1000, 42);
            cis.push_back(ci);
            all_y_values.push_back(ci.low);
            all_y_values.push_back(ci.high);
        }
    }

    if (cat_names.empty()) {
        throw DataError("pointplot found no non-empty category groups");
    }

    
    // Configure Scales
    
    auto [x_min_px, x_max_px] = ax.x_pixel_range();
    auto [y_bottom_px, y_top_px] = ax.y_pixel_range();

    // X-axis: Categorical Scale mapping category indices [0..N-1] to pixels
    auto x_scale = std::make_shared<CategoricalScale>(cat_names, x_min_px, x_max_px);
    
    // Y-axis: Linear Scale over padded data bounds
    auto [y_domain_min, y_domain_max] = padded_range(all_y_values, 0.08);
    auto y_scale = std::make_shared<LinearScale>(y_domain_min, y_domain_max, y_bottom_px, y_top_px);

    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);

    
    // Draw Plot Frame (Background, Grid, Axes, Tick Marks/Labels)
    
    ax.draw_frame();

    Canvas& canvas = ax.canvas();

    
    // Render Plot Marks (Connecting Lines, CI Bars, Mean Markers)
    
    std::size_t n = cat_names.size();

    // 5a. Draw connecting lines between consecutive category means
    if (options.connect_points && n >= 2) {
        for (std::size_t i = 0; i < n - 1; ++i) {
            PointI p0 = ax.to_pixel(static_cast<double>(i), means[i]);
            PointI p1 = ax.to_pixel(static_cast<double>(i + 1), means[i + 1]);
            draw_line(canvas, p0.x, p0.y, p1.x, p1.y, options.color, options.line_thickness);
        }
    }

    // 5b. Draw vertical Confidence Interval error bars with end-caps
    if (options.show_ci) {
        for (std::size_t i = 0; i < n; ++i) {
            PointI p_low = ax.to_pixel(static_cast<double>(i), cis[i].low);
            PointI p_high = ax.to_pixel(static_cast<double>(i), cis[i].high);

            // Vertical line for confidence interval
            draw_line(canvas, p_low.x, p_low.y, p_high.x, p_high.y, options.color, options.line_thickness);

            // Lower horizontal end-cap
            if (options.cap_size > 0) {
                draw_line(canvas, p_low.x - options.cap_size, p_low.y,
                          p_low.x + options.cap_size, p_low.y,
                          options.color, options.line_thickness);
                // Upper horizontal end-cap
                draw_line(canvas, p_high.x - options.cap_size, p_high.y,
                          p_high.x + options.cap_size, p_high.y,
                          options.color, options.line_thickness);
            }
        }
    }

    // 5c. Draw circular markers for category means on top
    for (std::size_t i = 0; i < n; ++i) {
        PointI pt = ax.to_pixel(static_cast<double>(i), means[i]);
        fill_circle(canvas, pt.x, pt.y, options.marker_radius, options.color);
    }
}

} // namespace cppviz