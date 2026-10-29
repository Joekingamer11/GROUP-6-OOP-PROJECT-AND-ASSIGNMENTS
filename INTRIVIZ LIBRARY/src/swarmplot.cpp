#include "cppviz/swarmplot.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "cppviz/categorical_scale.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/error.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/scale.hpp"
#include "cppviz/ticks.hpp"
#include "cppviz/validation.hpp"

namespace cppviz {

std::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double center_x,
                                double band_width, int radius) {
    if (radius <= 0) {
        throw InvalidArgument("radius must be positive");
    }
    if (band_width <= 0.0) {
        throw InvalidArgument("band_width must be positive");
    }
    if (y_pixels.empty()) {
        return {};
    }

    struct PointEntry {
        double y;
        std::size_t original_index;
    };

    std::vector<PointEntry> entries;
    entries.reserve(y_pixels.size());
    for (std::size_t i = 0; i < y_pixels.size(); ++i) {
        entries.push_back({y_pixels[i], i});
    }

    std::sort(entries.begin(), entries.end(), [](const PointEntry& a, const PointEntry& b) {
        return a.y < b.y;
    });

    const double min_dist_sq = 4.0 * radius * radius;
    const double max_offset = std::max(0.0, (band_width / 2.0) - radius);
    const double step = std::max(1.0, static_cast<double>(radius) * 0.5);

    std::vector<PointI> placed_points;
    placed_points.reserve(entries.size());
    std::vector<PointI> result(y_pixels.size());

    for (const auto& entry : entries) {
        double current_y = entry.y;
        double chosen_x = center_x;
        bool placed = false;

        for (int k = 0; ; ++k) {
            double offset = k * step;
            double candidate_offset = (k % 2 == 1) ? offset : -offset;

            if (k == 0) {
                candidate_offset = 0.0;
            }

            if (offset > max_offset) {
                candidate_offset = (k % 2 == 1) ? max_offset : -max_offset;
                chosen_x = center_x + candidate_offset;
                placed = true;
                break;
            }

            double cand_x = center_x + candidate_offset;
            bool collides = false;

            for (const auto& placed_pt : placed_points) {
                double dy = current_y - static_cast<double>(placed_pt.y);
                if (std::abs(dy) > 2.0 * radius) {
                    continue;
                }

                double dx = cand_x - static_cast<double>(placed_pt.x);
                if ((dx * dx + dy * dy) < min_dist_sq) {
                    collides = true;
                    break;
                }
            }

            if (!collides) {
                chosen_x = cand_x;
                placed = true;
                break;
            }
        }

        if (!placed) {
            chosen_x = center_x + max_offset;
        }

        PointI pt{static_cast<int>(std::round(chosen_x)),
                  static_cast<int>(std::round(current_y))};
        placed_points.push_back(pt);
        result[entry.original_index] = pt;
    }

    return result;
}

std::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double band_width, int radius) {
    return swarm_layout(y_pixels, 0.0, band_width, radius);
}

void swarmplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const SwarmplotOptions& options) {
    require_not_empty(data);
    require_text_column(data, x_column);
    require_numeric_column(data, y_column);

    if (options.marker_radius <= 0) {
        throw InvalidArgument("marker_radius must be positive (> 0)");
    }

    std::vector<Group> groups = group_by(data, x_column, y_column);
    if (groups.empty()) {
        throw DataError("swarmplot dataset contains no valid categories");
    }

    std::vector<std::string> cat_names;
    std::vector<double> all_y_values;
    cat_names.reserve(groups.size());

    for (const auto& group : groups) {
        if (group.values.empty()) {
            continue;
        }
        cat_names.push_back(group.key);
        all_y_values.insert(all_y_values.end(), group.values.begin(), group.values.end());
    }

    if (cat_names.empty() || all_y_values.empty()) {
        throw DataError("swarmplot dataset contains no observations to plot");
    }

    auto [x_min_px, x_max_px] = ax.x_pixel_range();
    auto [y_bottom_px, y_top_px] = ax.y_pixel_range();

    auto x_scale = std::make_shared<CategoricalScale>(cat_names, x_min_px, x_max_px);
    auto [y_dmin, y_dmax] = padded_range(all_y_values, 0.08);
    auto y_scale = std::make_shared<LinearScale>(y_dmin, y_dmax, y_bottom_px, y_top_px);

    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);
    ax.draw_frame();

    Canvas& canvas = ax.canvas();
    double band_w = x_scale->band_width();

    std::vector<Color> palette;
    if (options.color_by_category) {
        palette = categorical_palette("deep");
    }

    for (std::size_t cat_idx = 0; cat_idx < cat_names.size(); ++cat_idx) {
        const auto& group = groups[cat_idx];
        if (group.values.empty()) {
            continue;
        }

        Color dot_color = options.color_by_category ? palette_color(palette, cat_idx) : options.color;
        double center_x = x_scale->map(static_cast<double>(cat_idx));

        std::vector<double> y_pixels;
        y_pixels.reserve(group.values.size());
        for (double val : group.values) {
            y_pixels.push_back(y_scale->map(val));
        }

        std::vector<PointI> points = swarm_layout(y_pixels, center_x, band_w, options.marker_radius);
        for (const auto& pt : points) {
            fill_circle(canvas, pt.x, pt.y, options.marker_radius, dot_color);
        }
    }
}

} // namespace cppviz

    