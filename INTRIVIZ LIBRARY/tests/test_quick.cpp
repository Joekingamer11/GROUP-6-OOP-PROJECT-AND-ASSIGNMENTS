#include "mini_test.hpp"
#include "cppviz/quick.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"
#include <cstdio>

int main() {
    using namespace cppviz;

    // 1. Prepare sample dataset
    Dataset data;
    data.add_text_column("day", {"Thu", "Fri", "Sat", "Thu", "Fri", "Sat"});
    data.add_numeric_column("bill", {10.5, 20.0, 15.0, 12.0, 25.0, 18.0});
    data.add_numeric_column("tip", {1.5, 3.0, 2.0, 1.8, 4.0, 2.5});

    // 2. Test quick_swarmplot
    Figure fig_swarm = quick_swarmplot(data, "day", "bill", "Bill by Day");
    CHECK_EQ(fig_swarm.canvas().width(), 640);
    CHECK_EQ(fig_swarm.canvas().height(), 480);
    fig_swarm.save("data/output/test_quick_swarm.bmp");

    // 3. Test quick_pointplot
    Figure fig_point = quick_pointplot(data, "day", "bill", "Average Bill");
    CHECK_EQ(fig_point.canvas().width(), 640);
    CHECK_EQ(fig_point.canvas().height(), 480);
    fig_point.save("data/output/test_quick_point.bmp");

    // 4. Test quick_heatmap
    Figure fig_heat = quick_heatmap(data, {"bill", "tip"}, "Correlation Heatmap");
    CHECK_EQ(fig_heat.canvas().width(), 640);
    CHECK_EQ(fig_heat.canvas().height(), 480);
    fig_heat.save("data/output/test_quick_heatmap.bmp");

    // 5. Test Exception Handling
    CHECK_THROWS(quick_swarmplot(data, "invalid_col", "bill"), DataError);
    CHECK_THROWS(quick_pointplot(data, "day", "invalid_col"), DataError);
    CHECK_THROWS(quick_heatmap(data, {}, "Empty Matrix"), DataError);
    CHECK_THROWS(quick_heatmap(data, {"day"}, "Text Col in Heatmap"), DataError);

    // Clean up temporary test output files
    std::remove("data/output/test_quick_swarm.bmp");
    std::remove("data/output/test_quick_point.bmp");
    std::remove("data/output/test_quick_heatmap.bmp");

    return finish();
}