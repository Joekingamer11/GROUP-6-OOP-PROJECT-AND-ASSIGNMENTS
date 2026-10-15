//ANNA KAGEMULO MTABAZI 25/T/08565/PS

#include "mini_test.hpp"
#include "cppviz/summary.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

#include <limits>
#include <string>
#include <vector>

int main() {
    using namespace cppviz;

    const double nan_val = std::numeric_limits<double>::quiet_NaN();

    // ------------------------------------------------------------
    // Test 1: Normal dataset with numeric and text columns
    // ------------------------------------------------------------
    {
        Dataset ds;
        ds.add_text_column("category", {"GroupA", "GroupB", "GroupC", "GroupD"});
        ds.add_numeric_column("score", {10.0, 20.0, 30.0, 40.0});

        auto summaries = describe(ds);

        // 1. Text columns should be automatically skipped
        CHECK_EQ(summaries.size(), 1);

        // 2. Verify numeric column stats
        const auto& s = summaries;
        CHECK_EQ(s.name, "score");
        CHECK_EQ(s.count, 4);
        CHECK_NEAR(s.mean, 25.0, 1e-6);
        CHECK_NEAR(s.min, 10.0, 1e-6);
        CHECK_NEAR(s.max, 40.0, 1e-6);
        CHECK_NEAR(s.median, 25.0, 1e-6);
        CHECK_NEAR(s.q25, 17.5, 1e-6);
        CHECK_NEAR(s.q75, 32.5, 1e-6);
        CHECK(s.stddev > 0.0);
    }

    // -------------------------------------------------------------
    // Test 2: Handling missing values (NaN)
    // -------------------------------------------------------------
    {
        Dataset ds;
        // 4 total rows, but 1 NaN entry
        ds.add_numeric_column("values", {1.0, nan_val, 3.0, 5.0});

        auto summaries = describe(ds);
        CHECK_EQ(summaries.size(), 1);

        const auto& s = summaries;
        // Count must accurately reflect non-NaN values
        CHECK_EQ(s.count, 3);
        CHECK_NEAR(s.mean, 3.0, 1e-6); // mean of {1.0, 3.0, 5.0}
        CHECK_NEAR(s.min, 1.0, 1e-6);
        CHECK_NEAR(s.max, 5.0, 1e-6);
        CHECK_NEAR(s.median, 3.0, 1e-6);
    }

    // -------------------------------------------------------------
    // Test 3: Formatting summaries into text table (to_string)
    // -------------------------------------------------------------
    {
        Dataset ds;
        ds.add_numeric_column("temp", {20.5, 22.0, 19.5});
        auto summaries = describe(ds);

        std::string table = to_string(summaries);

        // Verify key headers and content exist in the formatted ASCII output
        CHECK(table.find("Column") != std::string::npos);
        CHECK(table.find("Mean") != std::string::npos);
        CHECK(table.find("StdDev") != std::string::npos);
        CHECK(table.find("temp") != std::string::npos);

        // Verify empty summary list formatting
        std::string empty_table = to_string({});
        CHECK(empty_table.find("no numeric columns") != std::string::npos);
    }

    // -------------------------------------------------------------
    // Test 4: Edge Cases (Single Observation & All NaNs)
    // -------------------------------------------------------------
    {
        // Case A: Single observation (stddev should be 0.0, not crash/nan)
        Dataset ds_single;
        ds_single.add_numeric_column("single", {42.0});

        auto summaries_single = describe(ds_single);
        CHECK_EQ(summaries_single.size(), 1);
        CHECK_EQ(summaries_single.count, 1);
        CHECK_NEAR(summaries_single.mean, 42.0, 1e-6);
        CHECK_NEAR(summaries_single.stddev, 0.0, 1e-6);

        // Case B: Column where all values are missing
        Dataset ds_all_nan;
        ds_all_nan.add_numeric_column("all_nan", {nan_val, nan_val});

        auto summaries_all_nan = describe(ds_all_nan);
        CHECK_EQ(summaries_all_nan.size(), 1);
        CHECK_EQ(summaries_all_nan.count, 0);
        CHECK_NEAR(summaries_all_nan.mean, 0.0, 1e-6);
        CHECK_NEAR(summaries_all_nan.stddev, 0.0, 1e-6);

        // Case C: Empty Dataset
        Dataset ds_empty;
        auto summaries_empty = describe(ds_empty);
        CHECK(summaries_empty.empty());
    }

    return finish();
}