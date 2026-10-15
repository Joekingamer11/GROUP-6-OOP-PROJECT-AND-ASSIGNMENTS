#include "mini_test.hpp"
#include "cppviz/types.hpp"

int main() {
    using namespace cppviz;

    // Test Color defaults and equality operators
    Color default_color;
    CHECK_EQ(default_color.r, 0);
    CHECK_EQ(default_color.g, 0);
    CHECK_EQ(default_color.b, 0);

    Color red(255, 0, 0);
    Color red_copy(255, 0, 0);
    Color blue(0, 0, 255);

    CHECK(red == red_copy);
    CHECK(red != blue);

    // Test PointI and PointD defaults
    PointI pt_i;
    CHECK_EQ(pt_i.x, 0);
    CHECK_EQ(pt_i.y, 0);

    PointD pt_d;
    CHECK_EQ(pt_d.x, 0.0);
    CHECK_EQ(pt_d.y, 0.0);

    // Test Rect defaults
    Rect rect;
    CHECK_EQ(rect.x, 0);
    CHECK_EQ(rect.y, 0);
    CHECK_EQ(rect.w, 0);
    CHECK_EQ(rect.h, 0);

    // Test Margins defaults
    Margins margins;
    CHECK_EQ(margins.left, 70);
    CHECK_EQ(margins.right, 20);
    CHECK_EQ(margins.top, 40);
    CHECK_EQ(margins.bottom, 60);

    return finish();
}