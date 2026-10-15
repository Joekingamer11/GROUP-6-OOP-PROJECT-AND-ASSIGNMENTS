#include "mini_test.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

#include <limits>
#include <string>

int main() {
    using namespace cppviz;

    // 1. Empty dataset defaults
    Dataset ds;
    CHECK_EQ(ds.row_count(), 0);
    CHECK_EQ(ds.column_count(), 0);
    CHECK(ds.column_names().empty());

    // 2. Add numeric and text columns
    ds.add_numeric_column("bill", {10.5, 20.0, 15.2});
    ds.add_text_column("day", {"Thu", "Fri", "Sat"});

    CHECK_EQ(ds.row_count(), 3);
    CHECK_EQ(ds.column_count(), 2);
    CHECK(ds.has_column("bill"));
    CHECK(ds.has_column("day"));
    CHECK(!ds.has_column("tip"));

    CHECK(ds.is_numeric("bill"));
    CHECK(!ds.is_numeric("day"));

    // 3. Verify accessors
    CHECK_NEAR(ds.numeric("bill")[0], 10.5, 1e-9);
    CHECK_EQ(ds.text("day")[1], std::string("Fri"));

    // 4. Invalid inputs & type checks
    CHECK_THROWS(ds.add_numeric_column("bill", {5.0, 5.0, 5.0}), InvalidArgument); // Duplicate name
    CHECK_THROWS(ds.add_numeric_column("tip", {2.0}), InvalidArgument);             // Wrong length
    CHECK_THROWS(ds.text("bill"), DataError);                                      // Wrong type access
    CHECK_THROWS(ds.numeric("day"), DataError);                                     // Wrong type access
    CHECK_THROWS(ds.is_numeric("unknown"), DataError);                              // Unknown column

    // 5. Missing value helper
    CHECK(is_missing(std::numeric_limits<double>::quiet_NaN()));
    CHECK(!is_missing(42.0));

    // 6. Select rows and reordering
    Dataset sub = ds.select_rows({2, 0});
    CHECK_EQ(sub.row_count(), 2);
    CHECK_NEAR(sub.numeric("bill")[0], 15.2, 1e-9);
    CHECK_NEAR(sub.numeric("bill")[1], 10.5, 1e-9);
    CHECK_EQ(sub.text("day")[0], std::string("Sat"));
    CHECK_EQ(sub.text("day")[1], std::string("Thu"));

    // Out-of-bounds row selection
    CHECK_THROWS(ds.select_rows({5}), InvalidArgument);

    return finish();
}