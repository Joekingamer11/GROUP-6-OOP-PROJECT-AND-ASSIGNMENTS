#include "mini_test.hpp"

#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"
#include "intriviz/swarmplot.hpp"
#include "intriviz/theme.hpp"

int main() {
    using namespace intriviz;

    std::vector<double> y_pts = {100.0, 100.0, 100.0};
    auto layout = swarm_layout(y_pts, 150.0, 100.0, 4);
    CHECK_EQ(layout.size(), 3u);

    Dataset df;
    df.add_text_column("day", {"Thur", "Fri", "Thur", "Fri", "Sat"});
    df.add_numeric_column("bill", {16.99, 10.34, 21.01, 23.68, 24.59});

    Canvas canvas(400, 300, Color{255, 255, 255});
    Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

    SwarmplotOptions opts;
    opts.marker_radius = 4;
    opts.color_by_category = true;

    swarmplot(ax, df, "day", "bill", opts);
    {
        bool valid = true;
        try { ax.to_pixel(0.0, 0.0); } catch (...) { valid = false; }
        CHECK(valid);
    }

    {
        bool threw = false;
        try { swarmplot(ax, df, "nonexistent", "bill"); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { swarmplot(ax, df, "day", "nonexistent"); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { swarmplot(ax, df, "bill", "day"); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    Dataset empty_df;
    {
        bool threw = false;
        try { swarmplot(ax, empty_df, "day", "bill"); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { swarm_layout({10.0}, 100.0, 0); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { swarm_layout({10.0}, -10.0, 4); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    return finish();
}


