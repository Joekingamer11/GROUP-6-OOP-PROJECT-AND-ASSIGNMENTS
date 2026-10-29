#include "intriviz/pointplot.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "intriviz/categorical_scale.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/error.hpp"
#include "intriviz/groupby.hpp"
#include "intriviz/scale.hpp"
#include "intriviz/stats.hpp"
#include "intriviz/ticks.hpp"
#include "intriviz/validation.hpp"

namespace intriviz {

void pointplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const PointplotOptions& options) {
    // Step 1: Input Validation
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    if (options.ci_level <= 0.0 || options.ci_level >= 1.0) {
        throw InvalidArgument("ci_level must be strictly between 0.0 and 1.0");
    }

    // Step 2: Group Data and Compute Category Statistics
    std::vector<Group> groups = group_by(data, x_column, y_column);
    if (groups.empty()) {
        throw DataError("pointplot dataset has no valid observation groups");
    }

    std::vector<std::string> cat_names;
    std::vector<double> means;
    std::vector<Interval> cis;
    std::vector<double> all_y_values;

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

    // Step 3: Configure Axis Scales
    auto [x_min_px, x_max_px] = ax.x_pixel_range();
    auto [y_bottom_px, y_top_px] = ax.y_pixel_range();

    auto x_scale = std::make_shared<CategoricalScale>(cat_names, x_min_px, x_max_px);
    auto [y_domain_min, y_domain_max] = padded_range(all_y_values, 0.08);
    auto y_scale = std::make_shared<LinearScale>(y_domain_min, y_domain_max, y_bottom_px, y_top_px);

    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);

    // Step 4: Draw Frame
    ax.draw_frame();

    Canvas& canvas = ax.canvas();
    std::size_t n = cat_names.size();

    // Step 5: Render Marks
    // 5a. Draw connecting lines between consecutive category means
    if (options.connect_points && n >= 2) {
        for (std::size_t i = 0; i < n - 1; ++i) {
            PointI p0 = ax.to_pixel(static_cast<double>(i), means[i]);
            PointI p1 = ax.to_pixel(static_cast<double>(i + 1), means[i + 1]);
            draw_line(canvas, p0.x, p0.y, p1.x, p1.y, options.color, options.line_thickness);
        }
    }

    // 5b. Draw vertical confidence interval bars with end-caps
    if (options.show_ci) {
        for (std::size_t i = 0; i < n; ++i) {
            PointI p_low = ax.to_pixel(static_cast<double>(i), cis[i].low);
            PointI p_high = ax.to_pixel(static_cast<double>(i), cis[i].high);

            // Vertical confidence interval line
            draw_line(canvas, p_low.x, p_low.y, p_high.x, p_high.y, options.color, options.line_thickness);

            // End-caps
            if (options.cap_size > 0) {
                draw_line(canvas, p_low.x - options.cap_size, p_low.y,
                          p_low.x + options.cap_size, p_low.y,
                          options.color, options.line_thickness);
                draw_line(canvas, p_high.x - options.cap_size, p_high.y,
                          p_high.x + options.cap_size, p_high.y,
                          options.color, options.line_thickness);
            }
        }
    }

    // 5c. Draw mean point markers on top
    for (std::size_t i = 0; i < n; ++i) {
        PointI pt = ax.to_pixel(static_cast<double>(i), means[i]);
        fill_circle(canvas, pt.x, pt.y, options.marker_radius, options.color);
    }
}

} // namespace intriviz
