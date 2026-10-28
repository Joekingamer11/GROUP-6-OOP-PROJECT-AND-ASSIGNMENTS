#pragma once

#include <string>
#include <vector>

#include "cppviz/axes.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/types.hpp"

namespace cppviz {

struct SwarmplotOptions {
    int marker_radius = 4;
    bool color_by_category = true;
    Color color{76, 114, 176};
};

// Overload calculating offsets relative to center_x = 0.0
std::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double center_x,
                                double band_width, int radius);

// Renders categorical swarm plot where points spread sideways to prevent overlap.
std::vector<PointI> swarm_layout(const std::vector<double>& y_pixels, double band_width, int radius);

void swarmplot(Axes& ax, const Dataset& data, const std::string& x_column,
               const std::string& y_column, const SwarmplotOptions& options = {});

} // namespace cppviz