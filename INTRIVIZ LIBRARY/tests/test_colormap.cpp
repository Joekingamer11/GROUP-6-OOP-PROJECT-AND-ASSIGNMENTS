#include "mini_test.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // 1. Hex parsing and formatting
    Color red = color_from_hex("#FF0000");
    CHECK_EQ(red.r, 255);
    CHECK_EQ(red.g, 0);
    CHECK_EQ(red.b, 0);

    Color green = color_from_hex("00FF00");
    CHECK_EQ(green.r, 0);
    CHECK_EQ(green.g, 255);
    CHECK_EQ(green.b, 0);

    CHECK_EQ(color_to_hex(Color{255, 0, 0}), std::string("#FF0000"));
    CHECK_EQ(color_to_hex(Color{76, 114, 176}), std::string("#4C72B0"));

    // Invalid hex error handling
    CHECK_THROWS(color_from_hex("#FF000"), InvalidArgument);
    CHECK_THROWS(color_from_hex("#GG0000"), InvalidArgument);

    // 2. Color blending
    Color black{0, 0, 0};
    Color white{255, 255, 255};
    Color mid = blend(black, white, 0.5);
    CHECK_EQ(mid.r, 128);
    CHECK_EQ(mid.g, 128);
    CHECK_EQ(mid.b, 128);

    // Out-of-bounds clamping in blend
    Color clamped_low = blend(black, white, -1.0);
    CHECK_EQ(clamped_low.r, 0);
    Color clamped_high = blend(black, white, 2.0);
    CHECK_EQ(clamped_high.r, 255);

    // 3. Categorical palette ("deep")
    auto deep = categorical_palette("deep");
    CHECK_EQ(deep.size(), static_cast<std::size_t>(10));
    CHECK_EQ(deep[0], color_from_hex("#4C72B0"));
    CHECK_THROWS(categorical_palette("unknown_palette"), InvalidArgument);

    // 4. Colormap class
    Colormap cm({black, white});
    CHECK_EQ(cm.at(0.0), black);
    CHECK_EQ(cm.at(1.0), white);
    CHECK_EQ(cm.at(-0.5), black);
    CHECK_EQ(cm.at(1.5), white);

    // Colormap requires at least 2 stops
    CHECK_THROWS(Colormap({black}), InvalidArgument);

    return finish();
}