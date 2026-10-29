#include "mini_test.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // --- Week 1 Tests ---
    Color red = color_from_hex("#FF0000");
    CHECK_EQ(red.r, 255);
    CHECK_EQ(red.g, 0);
    CHECK_EQ(red.b, 0);

    CHECK_EQ(color_to_hex(Color{76, 114, 176}), std::string("#4C72B0"));
    CHECK_THROWS(color_from_hex("#FF000"), InvalidArgument);

    Color black{0, 0, 0};
    Color white{255, 255, 255};
    Color mid = blend(black, white, 0.5);
    CHECK_EQ(mid.r, 128);

    auto deep = categorical_palette("deep");
    CHECK_EQ(deep.size(), static_cast<std::size_t>(10));

    Colormap custom_cm({black, white});
    CHECK_EQ(custom_cm.at(0.0), black);
    CHECK_EQ(custom_cm.at(1.0), white);

    // --- Week 2 Tests ---

    // 1. Named Colormaps (viridis, magma, coolwarm, gray)
    Colormap viridis = make_colormap("viridis");
    CHECK_EQ(viridis.at(0.0), color_from_hex("#440154"));
    CHECK_EQ(viridis.at(1.0), color_from_hex("#FDE725"));

    Colormap magma = make_colormap("magma");
    CHECK_EQ(magma.at(0.0), color_from_hex("#000004"));
    CHECK_EQ(magma.at(1.0), color_from_hex("#FCFDBF"));

    Colormap coolwarm = make_colormap("coolwarm");
    CHECK_EQ(coolwarm.at(0.0), color_from_hex("#3B4CC0"));
    CHECK_EQ(coolwarm.at(1.0), color_from_hex("#B40426"));

    Colormap gray = make_colormap("gray");
    CHECK_EQ(gray.at(0.0), black);
    CHECK_EQ(gray.at(1.0), white);

    CHECK_THROWS(make_colormap("unknown_map"), InvalidArgument);

    // 2. Palette wrap-around indexing
    CHECK_EQ(palette_color(deep, 0), deep[0]);
    CHECK_EQ(palette_color(deep, 10), deep[0]); // Wrap-around check
    CHECK_EQ(palette_color(deep, 12), deep[2]);
    CHECK_THROWS(palette_color({}, 0), InvalidArgument);

    // 3. Luminance & Contrast Text Selection
    CHECK_NEAR(luminance(black), 0.0, 1e-5);
    CHECK_NEAR(luminance(white), 255.0, 1e-5);

    // Dark background -> White text
    CHECK_EQ(contrast_text_color(black), white);
    // Light background -> Black text
    CHECK_EQ(contrast_text_color(white), black);

    return finish();
}