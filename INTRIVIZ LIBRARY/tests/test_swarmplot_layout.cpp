#include <cmath>
#include <vector>

#include "mini_test.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/swarmplot.hpp"
#include "intriviz/error.hpp"

int main() {
    using namespace intriviz;

    // --- Test 1: Minimum distance verification ---
    // Swarm dots in the same category must not overlap (distance >= 2 * radius).
    std::vector<double> y_pixels = {100.0, 110.0, 120.0, 130.0, 140.0};
    const int radius = 4;

    std::vector<PointI> points = swarm_layout(y_pixels, 80.0, 50.0, radius);
    CHECK_EQ(points.size(), static_cast<std::size_t>(5));

    const int min_dist_sq = (2 * radius) * (2 * radius);
    for (std::size_t i = 0; i < points.size(); ++i) {
        for (std::size_t j = i + 1; j < points.size(); ++j) {
            const int dx = points[i].x - points[j].x;
            const int dy = points[i].y - points[j].y;
            const int dist_sq = dx * dx + dy * dy;
            CHECK(dist_sq >= min_dist_sq);
        }
    }

    // --- Test 2: Single element per category ---
    std::vector<double> single_values = {15.0, 25.0};
    std::vector<PointI> single_points = swarm_layout(single_values, 0.0, 60.0, radius);
    CHECK_EQ(single_points.size(), static_cast<std::size_t>(2));

    // --- Test 3: Invalid input handling ---
    CHECK_THROWS(swarm_layout({10.0, 20.0}, 0.0, 0.0, radius), InvalidArgument);

    return finish();
}