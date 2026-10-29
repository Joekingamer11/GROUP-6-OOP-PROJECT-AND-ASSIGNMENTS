#include "mini_test.hpp"
#include "intriviz/font.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/types.hpp"

#include <string>

int main() {
    using namespace intriviz;

    // 1. Widths and heights
    CHECK_EQ(text_width(""), 0);
    CHECK_EQ(text_width("A"), 6);
    CHECK_EQ(text_height(1), 7);
    CHECK_EQ(text_width("AB"), 2 * text_width("A"));
    CHECK_EQ(text_width("AB"), 12);
    CHECK_EQ(text_width("A", 2), 12);
    CHECK_EQ(text_height(2), 14);

    // 2. Letter I should only touch its own cell
    Canvas canvas_i(30, 20, Color{0, 0, 0});
    Color text_color{255, 255, 255};
    draw_text(canvas_i, 5, 5, "I", text_color, 1);

    // Nothing should spill left or right of the cell
    CHECK((canvas_i.get_pixel(4, 5) == Color{0, 0, 0}));
    CHECK((canvas_i.get_pixel(11, 5) == Color{0, 0, 0}));

    // Columns 0 and 4 of the 'I' glyph are empty
    for (int y = 5; y <= 11; ++y) {
        CHECK((canvas_i.get_pixel(5, y) == Color{0, 0, 0}));
        CHECK((canvas_i.get_pixel(9, y) == Color{0, 0, 0}));
    }

    // The gap column after the character is untouched
    for (int y = 5; y <= 11; ++y) {
        CHECK((canvas_i.get_pixel(10, y) == Color{0, 0, 0}));
    }

    // The stem of the 'I' runs down column 2 (x = 7)
    for (int y = 5; y <= 11; ++y) {
        CHECK((canvas_i.get_pixel(7, y) == text_color));
    }

    // 3. Empty strings leave the canvas alone
    Canvas canvas_empty(20, 20, Color{0, 0, 0});
    draw_text(canvas_empty, 2, 2, "", text_color, 1);
    draw_text_vertical(canvas_empty, 2, 2, "", text_color, 1);
    CHECK((canvas_empty.get_pixel(2, 2) == Color{0, 0, 0}));

    // 4. Text near or off the edge should not crash
    Canvas canvas_clip(15, 15, Color{0, 0, 0});
    draw_text(canvas_clip, -5, -2, "TEST", text_color, 1);
    draw_text(canvas_clip, 12, 10, "OVERFLOW", text_color, 2);
    draw_text(canvas_clip, 100, 100, "OUTSIDE", text_color, 1);
    CHECK_EQ(canvas_clip.width(), 15);

    // 5. Vertical text lives in its own bounding box
    Canvas canvas_v(50, 50, Color{0, 0, 0});
    draw_text_vertical(canvas_v, 10, 10, "AB", text_color, 1);

    // For "AB" the box is [10..16] x [10..21]
    for (int x = 0; x < 50; ++x) {
        for (int y = 0; y < 50; ++y) {
            if (x < 10 || x > 16 || y < 10 || y > 21) {
                CHECK((canvas_v.get_pixel(x, y) == Color{0, 0, 0}));
            }
        }
    }

    return finish();
}