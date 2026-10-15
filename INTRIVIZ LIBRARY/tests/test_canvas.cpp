#include "mini_test.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/error.hpp"
#include "cppviz/types.hpp"

int main() {
    using namespace cppviz;

    const Color black{0, 0, 0};
    const Color white{255, 255, 255};

    // 1. Normal construction & background colour verification across all 4 corners
    {
        Color bg{240, 240, 240};
        Canvas canvas(100, 50, bg);

        CHECK_EQ(canvas.width(), 100);
        CHECK_EQ(canvas.height(), 50);
        CHECK(canvas.data().size() == 100 * 50 * 3);

        CHECK(canvas.get_pixel(0, 0) == bg);
        CHECK(canvas.get_pixel(99, 0) == bg);
        CHECK(canvas.get_pixel(0, 49) == bg);
        CHECK(canvas.get_pixel(99, 49) == bg);
    }

    // 2. Default background colour is white (255, 255, 255)
    {
        Canvas canvas(10, 10);
        CHECK(canvas.get_pixel(5, 5) == white);
    }

    // 3. set_pixel and get_pixel round-trip
    {
        Canvas canvas(20, 20, black);
        Color red{255, 0, 0};
        Color green{0, 255, 0};
        Color blue{0, 0, 255};

        canvas.set_pixel(0, 0, red);
        canvas.set_pixel(10, 10, green);
        canvas.set_pixel(19, 19, blue);

        CHECK(canvas.get_pixel(0, 0) == red);
        CHECK(canvas.get_pixel(10, 10) == green);
        CHECK(canvas.get_pixel(19, 19) == blue);
        CHECK(canvas.get_pixel(1, 1) == black);
    }

    // 4. Out-of-bounds drawing is silently ignored (clipping)
    {
        Canvas canvas(10, 10, black);
        canvas.set_pixel(-1, 5, white);
        canvas.set_pixel(10, 5, white);
        canvas.set_pixel(5, -1, white);
        canvas.set_pixel(5, 10, white);
        canvas.set_pixel(100, 100, white);

        CHECK(canvas.get_pixel(0, 5) == black);
        CHECK(canvas.get_pixel(9, 5) == black);
        CHECK(canvas.get_pixel(5, 0) == black);
        CHECK(canvas.get_pixel(5, 9) == black);
    }

    // 5. Out-of-bounds reading strictly throws InvalidArgument
    {
        Canvas canvas(10, 10);
        CHECK_THROWS(canvas.get_pixel(-1, 0), InvalidArgument);
        CHECK_THROWS(canvas.get_pixel(0, -1), InvalidArgument);
        CHECK_THROWS(canvas.get_pixel(10, 0), InvalidArgument);
        CHECK_THROWS(canvas.get_pixel(0, 10), InvalidArgument);
        CHECK_THROWS(canvas.get_pixel(50, 50), InvalidArgument);
    }

    // 6. fill() replaces all pixels
    {
        Canvas canvas(5, 5, black);
        Color yellow{255, 255, 0};
        canvas.fill(yellow);

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                CHECK(canvas.get_pixel(x, y) == yellow);
            }
        }
    }

    // 7. Invalid dimensions throw InvalidArgument
    {
        CHECK_THROWS(Canvas(0, 10), InvalidArgument);
        CHECK_THROWS(Canvas(10, 0), InvalidArgument);
        CHECK_THROWS(Canvas(-5, 10), InvalidArgument);
        CHECK_THROWS(Canvas(10, -5), InvalidArgument);
        CHECK_THROWS(Canvas(0, 0), InvalidArgument);
    }

    // 8. Raw data byte array size and ordering: (y * w + x) * 3
    {
        Canvas canvas(2, 2, Color{10, 20, 30});
        const auto& bytes = canvas.data();
        CHECK_EQ(bytes.size(), 2 * 2 * 3);

        // Row 0, col 0
        CHECK_EQ(bytes[0], 10);
        CHECK_EQ(bytes[1], 20);
        CHECK_EQ(bytes[2], 30);

        // Set row 1, col 0 -> index = (1 * 2 + 0) * 3 = 6
        canvas.set_pixel(0, 1, Color{100, 150, 200});
        CHECK_EQ(canvas.data()[6], 100);
        CHECK_EQ(canvas.data()[7], 150);
        CHECK_EQ(canvas.data()[8], 200);
    }

    return finish();
}

