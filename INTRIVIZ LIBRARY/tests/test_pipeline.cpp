#include "mini_test.hpp"
#include "cppviz/csv_loader.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/preprocess.hpp"
#include "cppviz/stats.hpp"
#include <string>
#include <vector>

int main() {
    using namespace cppviz;

    // 1. Parse raw CSV text containing missing/null values
    std::string csv_data = 
        "group,score,age\n"
        "A,85.5,23\n"
        "B,90.0,NA\n"       // NA missing
        "A,75.0,21\n"
        "B,,25\n"           // empty cell missing
        "A,95.0,22\n"
        "C,88.0,24\n";

    Dataset raw = parse_csv(csv_data);
    CHECK_EQ(raw.row_count(), 6u);
    CHECK_EQ(raw.column_count(), 3u);

    // 2. Preprocess: drop rows containing missing values
    Dataset clean = drop_missing(raw);
    CHECK_EQ(clean.row_count(), 4u); // Rows 2 (NA) and 4 (empty) dropped

    // 3. Group numerical scores by categorical 'group'
    auto groups = group_by(clean, "group", "score");
    CHECK_EQ(groups.size(), 2u); // Groups A and C remain (B had all incomplete rows)

    // Verify Group A values
    CHECK_EQ(groups[0].key, "A");
    CHECK_EQ(groups[0].values.size(), 3u);

    // 4. Compute statistical summaries on grouped values
    double mean_a = mean(groups[0].values);
    CHECK_NEAR(mean_a, 85.16666666666667, 1e-5); // (85.5 + 75.0 + 95.0) / 3

    double min_a = min_value(groups[0].values);
    CHECK_EQ(min_a, 75.0);

    double max_a = max_value(groups[0].values);
    CHECK_EQ(max_a, 95.0);

    // Verify Group C values
    CHECK_EQ(groups[1].key, "C");
    CHECK_EQ(groups[1].values.size(), 1u);
    CHECK_EQ(mean(groups[1].values), 88.0);

    return finish();
}