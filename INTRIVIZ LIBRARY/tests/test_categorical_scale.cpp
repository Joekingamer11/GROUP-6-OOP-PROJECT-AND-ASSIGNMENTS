#include "mini_test.hpp"
#include "intriviz/categorical_scale.hpp"
#include "intriviz/error.hpp"

int main() {
    using namespace intriviz;

    // 1. Three categories over 0..300 pixels
    std::vector<std::string> days = {"Thu", "Fri", "Sat"};
    CategoricalScale scale(days, 0.0, 300.0);

    CHECK_EQ(scale.category_count(), 3u);
    CHECK_NEAR(scale.band_width(), 100.0, 1e-9);

    // Category centers must land at 50, 150, 250
    CHECK_NEAR(scale.map(0.0), 50.0, 1e-9);
    CHECK_NEAR(scale.map(1.0), 150.0, 1e-9);
    CHECK_NEAR(scale.map(2.0), 250.0, 1e-9);

    // Invert mapping
    CHECK_NEAR(scale.invert(50.0), 0.0, 1e-9);
    CHECK_NEAR(scale.invert(150.0), 1.0, 1e-9);

    // Tick labels and values
    CHECK_EQ(scale.tick_label(0.0), "Thu");
    CHECK_EQ(scale.tick_label(1.0), "Fri");
    CHECK_EQ(scale.tick_label(2.0), "Sat");

    auto ticks = scale.tick_values();
    CHECK_EQ(ticks.size(), 3u);
    CHECK_NEAR(ticks[0], 0.0, 1e-9);
    CHECK_NEAR(ticks[1], 1.0, 1e-9);
    CHECK_NEAR(ticks[2], 2.0, 1e-9);

    // 2. Empty list must throw InvalidArgument
    CHECK_THROWS(CategoricalScale({}, 0.0, 300.0), InvalidArgument);

    return finish();
}