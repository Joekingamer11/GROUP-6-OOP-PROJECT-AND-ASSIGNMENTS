#include "mini_test.hpp"
#include "intriviz/figure.hpp"
#include "intriviz/error.hpp"
#include <fstream>

namespace {

// Helper function to test moving a Figure out of a function scope
intriviz::Figure create_custom_figure() {
    intriviz::Figure fig(500, 400);
    fig.set_margins(intriviz::Margins{50, 50, 30, 30});
    return fig;
}

} // anonymous namespace

int main() {
    using namespace intriviz;

    // 1. Test default constructor dimensions & default margins
    {
        Figure fig;
        CHECK_EQ(fig.canvas().width(), 640);
        CHECK_EQ(fig.canvas().height(), 480);

        // Default margins: left=70, right=20, top=40, bottom=60
        // Expected plot area: x=70, y=40, w=640-70-20=550, h=480-40-60=380
        const Rect& area = fig.axes().area();
        CHECK_EQ(area.x, 70);
        CHECK_EQ(area.y, 40);
        CHECK_EQ(area.w, 550);
        CHECK_EQ(area.h, 380);
    }

    // 2. Test set_margins and custom areas
    {
        Figure fig(400, 300);
        fig.set_margins(Margins{40, 40, 20, 20});
        
        const Rect& area = fig.axes().area();
        CHECK_EQ(area.x, 40);
        CHECK_EQ(area.y, 20);
        CHECK_EQ(area.w, 320); // 400 - 40 - 40
        CHECK_EQ(area.h, 260); // 300 - 20 - 20
    }

    // 3. Test invalid margins exception (area < 10x10)
    {
        // Canvas too small for default margins (10x10 width/height)
        {
            bool threw = false;
            try { Figure bad(50, 50); (void)bad; } catch (const InvalidArgument&) { threw = true; } catch (...) {}
            CHECK(threw);
        }

        // Margins consuming too much width (left 50 + right 50 leaves 0 width)
        {
            Figure fig(200, 200);
            bool threw = false;
            try { fig.set_margins(Margins{100, 100, 100, 100}); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
            CHECK(threw);
        }
    }

    // 4. Test move semantics & validity after function return
    {
        Figure fig = create_custom_figure();
        CHECK_EQ(fig.canvas().width(), 500);
        CHECK_EQ(fig.canvas().height(), 400);

        // Verify that axes still references valid canvas memory
        const Rect& area = fig.axes().area();
        CHECK_EQ(area.w, 400); // 500 - 50 - 50
        CHECK_EQ(area.h, 340); // 400 - 30 - 30

        // Test draw call on moved figure
        fig.axes().canvas().set_pixel(10, 10, Color{255, 0, 0});
        CHECK((fig.canvas().get_pixel(10, 10) == Color{255, 0, 0}));
    }

    // 5. Test saving figure to image file
    {
        Figure fig(200, 200);
        std::string test_path = "test_figure_output.bmp";
        fig.save(test_path);

        // Verify file exists and is non-empty
        std::ifstream file(test_path, std::ios::binary | std::ios::ate);
        CHECK(file.is_open());
        CHECK(file.tellg() > 54); // BMP header is 54 bytes
        file.close();

        // Cleanup temp test file
        std::remove(test_path.c_str());
    }

    return finish();
}