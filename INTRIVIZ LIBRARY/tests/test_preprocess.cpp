#include "mini_test.hpp"
#include "cppviz/preprocess.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"
#include <cmath>
#include <limits>

int main() {
    using namespace cppviz;

    // Set up test dataset
    Dataset ds;
    ds.add_text_column("category", {"A", "B", "", "D", "E"});
    ds.add_numeric_column("score", {10.0, 20.0, 30.0, std::numeric_limits<double>::quiet_NaN(), 50.0});
    ds.add_numeric_column("val", {1.0, 2.0, 3.0, 4.0, 5.0});

    // 1. Test drop_missing
    Dataset clean = drop_missing(ds);
    CHECK_EQ(clean.row_count(), 3u);
    CHECK_EQ(clean.text("category"), (std::vector<std::string>{"A", "B", "E"}));
    CHECK_NEAR(clean.numeric("score")[0], 10.0, 1e-9);
    CHECK_NEAR(clean.numeric("score")[1], 20.0, 1e-9);
    CHECK_NEAR(clean.numeric("score")[2], 50.0, 1e-9);

    // 2. Test fill_missing
    Dataset filled = fill_missing(ds, "score", 99.0);
    CHECK_EQ(filled.row_count(), 5u);
    CHECK_NEAR(filled.numeric("score")[3], 99.0, 1e-9);

    // Test fill_missing error cases
    CHECK_THROWS(fill_missing(ds, "nonexistent", 0.0), DataError);
    CHECK_THROWS(fill_missing(ds, "category", 0.0), DataError);

    // 3. Test filter_rows
    Dataset filtered = filter_rows(ds, "val", [](double v) { return v > 2.5; });
    CHECK_EQ(filtered.row_count(), 3u);
    CHECK_NEAR(filtered.numeric("val")[0], 3.0, 1e-9);

    // Filter error cases
    CHECK_THROWS(filter_rows(ds, "unknown", [](double) { return true; }), DataError);
    CHECK_THROWS(filter_rows(ds, "category", [](double) { return true; }), DataError);

    // 4. Test select_columns
    Dataset sub = select_columns(ds, {"val", "category"});
    CHECK_EQ(sub.column_count(), 2u);
    CHECK_EQ(sub.column_names(), (std::vector<std::string>{"val", "category"}));
    CHECK_THROWS(select_columns(ds, {"val", "missing"}), DataError);

    // 5. Test normalize_minmax
    std::vector<double> vec = {2.0, 4.0, 6.0};
    auto norm = normalize_minmax(vec);
    CHECK_EQ(norm.size(), 3u);
    CHECK_NEAR(norm[0], 0.0, 1e-9);
    CHECK_NEAR(norm[1], 0.5, 1e-9);
    CHECK_NEAR(norm[2], 1.0, 1e-9);

    // normalize_minmax error on constant or empty vector
    CHECK_THROWS(normalize_minmax({5.0, 5.0, 5.0}), DataError);
    CHECK_THROWS(normalize_minmax({}), DataError);

    // 6. Test standardize
    std::vector<double> std_vec = {10.0, 20.0, 30.0};
    auto zscores = standardize(std_vec);
    CHECK_EQ(zscores.size(), 3u);
    CHECK_NEAR(zscores[1], 0.0, 1e-9);

    // standardize error on constant or empty vector
    CHECK_THROWS(standardize({4.0, 4.0}), DataError);
    CHECK_THROWS(standardize({}), DataError);

    return finish();
}
