#include "mini_test.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/error.hpp"
#include "cppviz/types.hpp"

int main() {
    using namespace cppviz;

    Color black{0, 0, 0};
    Color white{255, 255, 255};
    Color red{255, 0, 0};
    Color green{0, 255, 0};
    Color blue{0, 0, 255};

    // 1. Horizontal, vertical, and diagonal lines hit endpoints and intermediate pixels
    {
        Canvas canvas(20, 20, black);

        // Horizontal line
        draw_line(canvas, 2, 4, 8, 4, red);
        CHECK(canvas.get_pixel(2, 4) == red);
        CHECK(canvas.get_pixel(5, 4) == red);
        CHECK(canvas.get_pixel(8, 4) == red);
        CHECK(canvas.get_pixel(2, 3) == black);
        CHECK(canvas.get_pixel(2, 5) == black);

        // Vertical line
        draw_line(canvas, 10, 3, 10, 9, green);
        CHECK(canvas.get_pixel(10, 3) == green);
        CHECK(canvas.get_pixel(10, 6) == green);
        CHECK(canvas.get_pixel(10, 9) == green);
        CHECK(canvas.get_pixel(9, 3) == black);
        CHECK(canvas.get_pixel(11, 3) == black);

        // Diagonal line
        draw_line(canvas, 12, 12, 17, 17, blue);
        CHECK(canvas.get_pixel(12, 12) == blue);
        CHECK(canvas.get_pixel(14, 14) == blue);
        CHECK(canvas.get_pixel(17, 17) == blue);
        CHECK(canvas.get_pixel(12, 13) == black);
    }

    // 2. Line drawn backwards (x0 > x1, y0 > y1) hits same endpoints
    {
        Canvas canvas(10, 10, black);
        draw_line(canvas, 7, 7, 3, 3, red);
        CHECK(canvas.get_pixel(7, 7) == red);
        CHECK(canvas.get_pixel(5, 5) == red);
        CHECK(canvas.get_pixel(3, 3) == red);
    }

    // 3. Line clipping (partially or completely outside canvas) does not crash
    {
        Canvas canvas(10, 10, black);
        draw_line(canvas, -5, -5, 15, 15, white);
        CHECK(canvas.get_pixel(0, 0) == white);
        CHECK(canvas.get_pixel(5, 5) == white);
        CHECK(canvas.get_pixel(9, 9) == white);

        draw_line(canvas, -20, 5, -5, 5, white); // completely off-canvas
    }

    // 4. Line thickness parameter
    {
        Canvas canvas(15, 15, black);
        draw_line(canvas, 5, 2, 5, 10, red, 3);
        // Center line
        CHECK(canvas.get_pixel(5, 5) == red);
        // Thickness stamps neighboring pixels
        CHECK(canvas.get_pixel(4, 5) == red);
        CHECK(canvas.get_pixel(6, 5) == red);
        CHECK(canvas.get_pixel(3, 5) == black);
        CHECK(canvas.get_pixel(7, 5) == black);
    }

    // 5. Rectangle outline (draw_rect)
    {
        Canvas canvas(20, 20, black);
        draw_rect(canvas, 4, 4, 6, 4, green);

        // Four corners
        CHECK(canvas.get_pixel(4, 4) == green);
        CHECK(canvas.get_pixel(9, 4) == green);
        CHECK(canvas.get_pixel(4, 7) == green);
        CHECK(canvas.get_pixel(9, 7) == green);

        // Edges
        CHECK(canvas.get_pixel(6, 4) == green);
        CHECK(canvas.get_pixel(4, 5) == green);

        // Interior remains untouched
        CHECK(canvas.get_pixel(5, 5) == black);
        CHECK(canvas.get_pixel(8, 6) == black);

        // Exterior remains untouched
        CHECK(canvas.get_pixel(3, 4) == black);
        CHECK(canvas.get_pixel(10, 4) == black);
    }

    // 6. Rectangle normalization for negative dimensions
    {
        Canvas canvas(10, 10, black);
        draw_rect(canvas, 6, 6, -3, -3, red);
        // Equivalent to x=3, y=3, w=3, h=3
        CHECK(canvas.get_pixel(3, 3) == red);
        CHECK(canvas.get_pixel(5, 3) == red);
        CHECK(canvas.get_pixel(3, 5) == red);
        CHECK(canvas.get_pixel(5, 5) == red);
        CHECK(canvas.get_pixel(4, 4) == black); // center
    }

    // 7. Solid filled rectangle (fill_rect)
    {
        Canvas canvas(10, 10, black);
        fill_rect(canvas, 2, 2, 4, 3, blue);

        // Check entire interior is colored
        for (int y = 2; y < 5; ++y) {
            for (int x = 2; x < 6; ++x) {
                CHECK(canvas.get_pixel(x, y) == blue);
            }
        }

        // Surrounding pixels are unchanged
        CHECK(canvas.get_pixel(1, 2) == black);
        CHECK(canvas.get_pixel(6, 2) == black);
        CHECK(canvas.get_pixel(2, 1) == black);
        CHECK(canvas.get_pixel(2, 5) == black);
    }

    // 8. Circle outline (draw_circle)
    {
        Canvas canvas(25, 25, black);
        int cx = 12, cy = 12, r = 6;
        draw_circle(canvas, cx, cy, r, red);

        // Cardinal points on perimeter
        CHECK(canvas.get_pixel(cx + r, cy) == red);
        CHECK(canvas.get_pixel(cx - r, cy) == red);
        CHECK(canvas.get_pixel(cx, cy + r) == red);
        CHECK(canvas.get_pixel(cx, cy - r) == red);

        // Center is hollow (not drawn)
        CHECK(canvas.get_pixel(cx, cy) == black);

        // Pixels outside circle are untouched
        CHECK(canvas.get_pixel(cx + r + 1, cy) == black);
        CHECK(canvas.get_pixel(cx, cy + r + 1) == black);
    }

    // 9. Solid filled circle (fill_circle)
    {
        Canvas canvas(25, 25, black);
        int cx = 12, cy = 12, r = 5;
        fill_circle(canvas, cx, cy, r, green);

        // Center is filled
        CHECK(canvas.get_pixel(cx, cy) == green);

        // Cardinal perimeter is filled
        CHECK(canvas.get_pixel(cx + r, cy) == green);
        CHECK(canvas.get_pixel(cx - r, cy) == green);
        CHECK(canvas.get_pixel(cx, cy + r) == green);
        CHECK(canvas.get_pixel(cx, cy - r) == green);

        // Diagonal within radius: (3^2 + 3^2 = 18 <= 25)
        CHECK(canvas.get_pixel(cx + 3, cy + 3) == green);

        // Beyond radius: (cx + r + 1, cy) or diagonal (cx + 4, cy + 4) -> 16+16=32 > 25
        CHECK(canvas.get_pixel(cx + r + 1, cy) == black);
        CHECK(canvas.get_pixel(cx + 4, cy + 4) == black);
    }

    // 10. Radius 0 and negative radius validation
    {
        Canvas canvas(10, 10, black);
        // Radius 0 draws a single pixel
        draw_circle(canvas, 5, 5, 0, white);
        CHECK(canvas.get_pixel(5, 5) == white);
        CHECK(canvas.get_pixel(4, 5) == black);

        fill_circle(canvas, 2, 2, 0, red);
        CHECK(canvas.get_pixel(2, 2) == red);

        // Negative radius throws InvalidArgument
        CHECK_THROWS(draw_circle(canvas, 5, 5, -1, white), InvalidArgument);
        CHECK_THROWS(fill_circle(canvas, 5, 5, -3, white), InvalidArgument);
    }

    return finish();
}