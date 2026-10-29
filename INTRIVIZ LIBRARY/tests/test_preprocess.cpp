#include "mini_test.hpp"
#include "intriviz/preprocess.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"
#include <cmath>
#include <limits>

int main() {
    using namespace intriviz;

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
    {
        bool threw = false;
        try { fill_missing(ds, "nonexistent", 0.0); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { fill_missing(ds, "category", 0.0); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // 3. Test filter_rows
    Dataset filtered = filter_rows(ds, "val", [](double v) { return v > 2.5; });
    CHECK_EQ(filtered.row_count(), 3u);
    CHECK_NEAR(filtered.numeric("val")[0], 3.0, 1e-9);

    // Filter error cases
    {
        bool threw = false;
        try { filter_rows(ds, "unknown", [](double) { return true; }); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { filter_rows(ds, "category", [](double) { return true; }); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // 4. Test select_columns
    Dataset sub = select_columns(ds, {"val", "category"});
    CHECK_EQ(sub.column_count(), 2u);
    CHECK_EQ(sub.column_names(), (std::vector<std::string>{"val", "category"}));
    {
        bool threw = false;
        try { select_columns(ds, {"val", "missing"}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // 5. Test normalize_minmax
    std::vector<double> vec = {2.0, 4.0, 6.0};
    auto norm = normalize_minmax(vec);
    CHECK_EQ(norm.size(), 3u);
    CHECK_NEAR(norm[0], 0.0, 1e-9);
    CHECK_NEAR(norm[1], 0.5, 1e-9);
    CHECK_NEAR(norm[2], 1.0, 1e-9);

    // normalize_minmax error on constant or empty vector
    {
        bool threw = false;
        try { normalize_minmax({5.0, 5.0, 5.0}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { normalize_minmax({}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // 6. Test standardize
    std::vector<double> std_vec = {10.0, 20.0, 30.0};
    auto zscores = standardize(std_vec);
    CHECK_EQ(zscores.size(), 3u);
    CHECK_NEAR(zscores[1], 0.0, 1e-9);

    // standardize error on constant or empty vector
    {
        bool threw = false;
        try { standardize({4.0, 4.0}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { standardize({}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    return finish();
}
