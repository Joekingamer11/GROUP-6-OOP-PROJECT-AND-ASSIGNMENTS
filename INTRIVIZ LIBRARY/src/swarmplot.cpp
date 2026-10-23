#include "cppviz/swarmplot.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "cppviz/categorial_scale.hpp
#include "cppviz/colormap.hpp"
#include "cppviz/draw.hpp" 
#include "cppviz/error.hpp" 
#include "cppviz/groupby.hpp" 
#include "cppviz/scale.hpp"
 #include "cppviz/ticks.hpp" 
 #include "cppviz/validation.hpp"

 namespace cppviz { 
    std::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double center_x, double band_width, int radius) {
         if (radius <= 0) {
            throw Invalid_argument("radius must be positive");
         }
         if (band_width <= 0) {
            throw Invalid_argument("band_width must be positive");
         }
         if (y_pixels.empty()) {
            return {};
         }
         //to pair each y_pixel with its original index
         struct PointEntry {
            double y;
            std::size_t original_index;
         };
         std::vector<PointEntry> entries;
         entries.reserve(y_pixels.size());
         for (std::size_t i = 0; i < y_pixels.size(); ++i) {
            entries.push_back({y_pixels[i], i});
         }
         //Sort observations along Y axis
         std::sort(entries.begin(), entries.end(), [](const PointEntry& a, const PointEntry& b) {
            return a.y < b.y;
         });
        const double min_dist_sq = 4.0 * radius * radius;
        const double max_offset = std::max(0.0, (band_width/2.0) - radius);
         const double step = std::max(1.0, static_cast<double>(radius) * 0.5);

         std::vector<PoinrI> placed_points;
         placed_points.reserve(entries.size());

         std::vector<PointI> result(y_pixels.sixe());
        
         for (const auto& entry : enrties) {
            double y = entry.y;
            double x = center_x;
            bool placed = false;

            //candidate offsets: o, +step, -step, ...

            for (int k = 0;  ; ++k) {
                double offset = k * step;
                if (offset > max_offset) {
                   
                 //limit reached: clamp to category band width
                chosen_x = (k % 2 ==1) ? center_x + max_offset : center_x - max_offset;
                 break;
                }
                // to alternate sign: 0, +offset, -offset, +2*offset, -2*offset, ...
                double candidate_offset = (k % 2 == 1) ? offset : -offset;

                if (k== 0) {
                    candidate_offset = 0.0; // first candidate is the center
                }

                double cand_x = center_x + candidate_offset;

                // check for collison against already placed points
                bool colllides = false;
                    for(const auto& placed : placed_points) {
                        //Optimisation: skip points that are too far away in the y direction
                        double dy = current_y - static_cast<double> (placed.y);

                        if (std::abs(dy) > 2.0 * radius) {
                            continue;
                        }

                        double dx = cand_x - static_cast<double>(placed.x);

                        if ((dx * dx + dy * dy) < min_dist_sq) {
                            collides = true;
                            break;
                        }
                    }
                        if (!collides) {
                            chosen_x = cand_x;
                            found = true;
                            break;
                        }
                        if (k == 0) {
                            continue;
                        }
                        
                    }
            PointI pt{static_cast<int>(std::round(chosen_x)),
                        static_cast<int>(std::round(current_y))};
            placed_points.push_back(pt);
            result[entry.original_index] = pt;
         }

         return result;

        }
        td::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double band_width, int radius) {
    return swarm_layout(y_pixels, 0.0, band_width, radius);
        }
        void swarmplot(Axes&amp; ax, const Dataset&amp; data, const std::string&amp; x_column, 
            const std::string&amp; y_column, const SwarmplotOptions&amp; options) {
                //Input Validation
                require_not_empty(data);
                require_text_column(date, x_column);
                require_numeric_number(date, y_column);

                if (options.marker_radius &lt;= 0) { 
                    throw InvalidArgument("marker_radius must be positive (&gt; 0)"); 
                }

                //Group observation by category
                std::vector<Group> groups = group_by(data, x_column, y_column);
    if (groups.empty()) {
        throw DataError("swarmplot dataset contains no valid categories");
    }

    std::vector<std::string> cat_names;
    std::vector<double> all_y_values;
    cat_names.reserve(groups.size());

    for (const auto& g : groups) {
        if (!g.values.empty()) {
            cat_names.push_back(g.key); 
            
            for (double val : g.values) 
            { all_y_values.push_back(val);
             } 
            } 
        
        } if (cat_names.empty() || all_y_values.empty()) { 
            throw DataError("swarmplot dataset contains no observations to plot");
         }

         //configure axis scales
         auto [x_min_px, x_max_px] = ax.x_pixel_range(); 
         auto [y_bottom_px, y_top_px] = ax.y_pixel_range();

        auto x_scale = std::make_shared<CategoricalScale>
        (cat_names, x_min_px, x_max_px); 

        auto [y_dmin, y_dmax] = padded_range(all_y_values, 0.08);
        auto y_scale = std::make_shared<LinearScale>(y_dmin, y_dmax, y_bottom_px, y_top_px);

    ax.set_x_scale(x_scale);
    ax.set_y_scale(y_scale);

         //Draw Plot Frame
         ax.draw_frame();

         //Render Observation Dots

         Canvas& canvas = ax,canvas();
         double band_w = x_scale->band_width();

         std::vector<Color> palette;
         if(options.color_by_category){
            palette = categorial_palette("deep");
         }
            for (std::size_t cat_idx = 0; cat_idx &lt; cat_names.size(); ++cat_idx) { 
                const auto&amp; group = groups[cat_idx]; 
                if (group.values.empty()) { 
                    continue; 
                }
        Color dot_color = options.color_by_category ? palette_color(palette, cat_idx) : options.color; 
        double center_x = x_scale-&gt;map(static_cast<double> (cat_idx));

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

}

    