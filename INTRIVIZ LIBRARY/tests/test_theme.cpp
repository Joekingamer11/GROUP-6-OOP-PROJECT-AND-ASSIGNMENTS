#include "mini_test.hpp"
#include "cppviz/theme.hpp"
#include "cppviz/types.hpp"

int main() {
    using namespace cppviz;

    // --- 1. theme_white() properties ---
    Theme white = theme_white();
    CHECK(!(white.show_grid));
    CHECK((white.background == Color{255, 255, 255}));
    CHECK((white.plot_background == Color{255, 255, 255}));
    CHECK((white.axis_color == Color{38, 38, 38}));
    CHECK((white.text_color == Color{38, 38, 38}));
    CHECK(white.font_scale >= 1);
    CHECK(white.marker_radius > 0);
    CHECK(white.line_thickness > 0);

    // --- 2. theme_whitegrid() properties ---
    Theme whitegrid = theme_whitegrid();
    CHECK(whitegrid.show_grid);
    CHECK((whitegrid.plot_background == Color{255, 255, 255}));
    CHECK((whitegrid.grid_color == Color{220, 220, 220}));

    // --- 3. theme_darkgrid() properties ---
    Theme darkgrid = theme_darkgrid();
    CHECK(darkgrid.show_grid);
    CHECK((darkgrid.plot_background == Color{234, 234, 242}));
    CHECK((darkgrid.grid_color == Color{255, 255, 255}));

    // --- 4. theme_dark() properties ---
    Theme dark = theme_dark();
    CHECK(!(dark.show_grid));
    // Dark theme background must be visibly darker than white theme background
    std::uint32_t dark_lum = dark.background.r + dark.background.g + dark.background.b;
    std::uint32_t white_lum = white.background.r + white.background.g + white.background.b;
    CHECK(dark_lum < white_lum);
    CHECK((dark.text_color == Color{220, 220, 220}));

    return finish();
}