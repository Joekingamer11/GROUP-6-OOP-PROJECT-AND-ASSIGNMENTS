#include "mini_test.hpp"
#include "cppviz/axes.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"
#include "cppviz/scale.hpp"
#include "cppviz/validation.hpp"
#include <limits>
#include <memory>

int main() {
    using namespace cppviz;

    // =========================================================================
    // 1. Validation Helper & Error Macro Invalid Inputs
    // =========================================================================

    // require() throwing InvalidArgument on false condition
    CHECK_THROWS(require(false, "custom validation failed"), InvalidArgument);

    // Setup dataset for validation tests
    Dataset data;
    data.add_numeric_column("bill", {10.5, 20.0, 15.2});
    data.add_text_column("day", {"Mon", "Tue", "Wed"});

    // require_column: missing column name
    CHECK_THROWS(require_column(data, "non_existent"), DataError);

    // require_numeric_column: text column or missing column
    CHECK_THROWS(require_numeric_column(data, "day"), DataError);
    CHECK_THROWS(require_numeric_column(data, "missing_col"), DataError);

    // require_text_column: numeric column or missing column
    CHECK_THROWS(require_text_column(data, "bill"), DataError);
    CHECK_THROWS(require_text_column(data, "missing_col"), DataError);

    // require_not_empty: empty dataset
    Dataset empty_data;
    CHECK_THROWS(require_not_empty(empty_data), DataError);

    // require_same_size: vector length mismatch
    CHECK_THROWS(require_same_size(5, 10, "vector comparison"), InvalidArgument);


    // =========================================================================
    // 2. Canvas & Pixel Out-of-Bounds Invalid Inputs
    // =========================================================================

    // Invalid canvas dimensions (width or height <= 0)
    CHECK_THROWS(Canvas(0, 100), InvalidArgument);
    CHECK_THROWS(Canvas(100, -5), InvalidArgument);

    Canvas canvas(100, 100);

    // get_pixel outside bounds (should throw InvalidArgument)
    CHECK_THROWS(canvas.get_pixel(-1, 50), InvalidArgument);
    CHECK_THROWS(canvas.get_pixel(50, -1), InvalidArgument);
    CHECK_THROWS(canvas.get_pixel(100, 50), InvalidArgument);
    CHECK_THROWS(canvas.get_pixel(50, 100), InvalidArgument);

    // set_pixel outside bounds (should safely ignore without crashing or throwing)
    canvas.set_pixel(-10, 50, Color{255, 0, 0});
    canvas.set_pixel(150, 50, Color{255, 0, 0});


    // =========================================================================
    // 3. Scale Domain & Range Boundary Invalid Inputs
    // =========================================================================

    // LinearScale with zero-span domain (domain_min == domain_max)
    CHECK_THROWS(LinearScale(10.0, 10.0, 0.0, 100.0), InvalidArgument);

    // LinearScale with NaN values in domain or range
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    CHECK_THROWS(LinearScale(nan_val, 10.0, 0.0, 100.0), InvalidArgument);
    CHECK_THROWS(LinearScale(0.0, 10.0, nan_val, 100.0), InvalidArgument);

    // LogScale with non-positive domain (domain_min <= 0 or domain_max <= 0)
    CHECK_THROWS(LogScale(0.0, 100.0, 0.0, 500.0), InvalidArgument);
    CHECK_THROWS(LogScale(-5.0, 10.0, 0.0, 500.0), InvalidArgument);


    // =========================================================================
    // 4. Axes Unset Scales & Uninitialized Coordinate Mappings
    // =========================================================================

    Rect area{50, 50, 400, 300};
    Theme theme = theme_white();
    Axes ax(canvas, area, theme);

    // Accessing scales before setting them
    CHECK_THROWS(ax.x_scale(), InvalidArgument);
    CHECK_THROWS(ax.y_scale(), InvalidArgument);

    // to_pixel before setting scales
    CHECK_THROWS(ax.to_pixel(1.0, 2.0), InvalidArgument);

    // Valid scales set successfully
    auto xs = std::make_shared<LinearScale>(0.0, 10.0, 50.0, 450.0);
    auto ys = std::make_shared<LinearScale>(0.0, 100.0, 350.0, 50.0);

    ax.set_x_scale(xs);
    ax.set_y_scale(ys);

    // Verify valid mapping after initialization
    PointI pt = ax.to_pixel(5.0, 50.0);
    CHECK_EQ(pt.x, 250);
    CHECK_EQ(pt.y, 200);

    return finish();
}