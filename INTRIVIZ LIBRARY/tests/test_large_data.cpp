#include "mini_test.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/preprocess.hpp"
#include "cppviz/stats.hpp"
#include <chrono>
#include <limits>
#include <string>
#include <vector>

int main() {
    using namespace cppviz;

    const std::size_t N = 10000;
    std::vector<std::string> categories;
    std::vector<double> values;
    categories.reserve(N);
    values.reserve(N);

    // Generate 10,000 synthetic rows across 4 categories
    const char* cat_names[] = {"North", "South", "East", "West"};
    for (std::size_t i = 0; i < N; ++i) {
        categories.push_back(cat_names[i % 4]);
        // Insert NaN every 100th row to test drop_missing at scale
        if (i % 100 == 0) {
            values.push_back(std::numeric_limits<double>::quiet_NaN());
        } else {
            values.push_back(static_cast<double>(i % 50) + 1.0);
        }
    }

    Dataset ds;
    ds.add_text_column("region", categories);
    ds.add_numeric_column("sales", values);

    CHECK_EQ(ds.row_count(), N);

    auto start_time = std::chrono::high_resolution_clock::now();

    // 1. Stress test drop_missing
    Dataset cleaned = drop_missing(ds);
    CHECK_EQ(cleaned.row_count(), N - 100);

    // 2. Stress test group_by
    auto groups = group_by(cleaned, "region", "sales");
    CHECK_EQ(groups.size(), 4u);

    // 3. Stress test statistical calculations across large groups
    for (const auto& grp : groups) {
        CHECK(grp.values.size() > 2000u);
        double m = mean(grp.values);
        CHECK(m > 0.0);
        double sd = stddev(grp.values);
        CHECK(sd > 0.0);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    // Ensure 10,000 rows complete in under 2000ms
    CHECK(duration_ms < 2000);

    return finish();
}