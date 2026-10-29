#include <cmath>
#include <vector>

#include "mini_test.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/swarmplot.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // --- Test 1: Minimum distance verification ---
    // Swarm dots in the same category must not overlap (distance >= 2 * radius)
    Dataset ds;
    ds.add_text_column("category", {"A", "A", "A", "A", "A"});
    ds.add_numeric_column("val", {10.0, 10.0, 10.0, 10.0, 10.0});

    SwarmplotOptions opts;
    opts.marker_radius = 4;

    // Compute pixel positions via swarm_layout helper
    std::vector<PointI> points = swarm_layout(ds, "category", "val", opts, 400, 300);
    CHECK_EQ(points.size(), static_cast<std::size_t>(5));

    int min_dist_sq = (2 * opts.marker_radius) * (2 * opts.marker_radius);
    for (std::size_t i = 0; i < points.size(); ++i) {
        for (std::size_t j = i + 1; j < points.size(); ++j) {
            int dx = points[i].x - points[j].x;
            int dy = points[i].y - points[j].y;
            int dist_sq = dx * dx + dy * dy;
            CHECK(dist_sq >= min_dist_sq);
        }
    }

    // --- Test 2: Single element per category ---
    Dataset ds_single;
    ds_single.add_text_column("group", {"Cat1", "Cat2"});
    ds_single.add_numeric_column("score", {15.0, 25.0});

    std::vector<PointI> single_points = swarm_layout(ds_single, "group", "score", opts, 400, 300);
    CHECK_EQ(single_points.size(), static_cast<std::size_t>(2));

    // --- Test 3: Invalid input handling ---
    Dataset empty_ds;
    CHECK_THROWS(swarm_layout(empty_ds, "cat", "val", opts, 400, 300), DataError);

    Dataset bad_col_ds;
    bad_col_ds.add_text_column("cat", {"A"});
    bad_col_ds.add_numeric_column("val", {1.0});
    CHECK_THROWS(swarm_layout(bad_col_ds, "missing", "val", opts, 400, 300), DataError);

    return finish();
}