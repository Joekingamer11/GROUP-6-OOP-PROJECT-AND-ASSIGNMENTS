#include "mini_test.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/error.hpp"

#include <limits>
#include <string>
#include <vector>

int main() {
    using namespace cppviz;

    // 1. Test unique_values (preserving first-appearance order)
    std::vector<std::string> items = {"Fri", "Thu", "Fri", "Sat", "Thu", "Sun"};
    std::vector<std::string> unique_res = unique_values(items);
    CHECK_EQ(unique_res.size(), 4);
    CHECK_EQ(unique_res[0], std::string("Fri"));
    CHECK_EQ(unique_res[1], std::string("Thu"));
    CHECK_EQ(unique_res[2], std::string("Sat"));
    CHECK_EQ(unique_res[3], std::string("Sun"));

    // 2. Test group_by with mixed order and NaN values
    Dataset ds;
    ds.add_text_column("day", {"Fri", "Thu", "Fri", "Sat", "Thu", "Fri"});
    ds.add_numeric_column("bill", {20.0, 15.0, std::numeric_limits<double>::quiet_NaN(), 30.0, 18.0, 25.0});

    std::vector<Group> groups = group_by(ds, "day", "bill");

    // Order of appearance: "Fri" first, then "Thu", then "Sat"
    CHECK_EQ(groups.size(), 3);

    CHECK_EQ(groups[0].key, std::string("Fri"));
    CHECK_EQ(groups[0].values.size(), 2); // 20.0 and 25.0 (NaN skipped)
    CHECK_NEAR(groups[0].values[0], 20.0, 1e-9);
    CHECK_NEAR(groups[0].values[1], 25.0, 1e-9);

    CHECK_EQ(groups[1].key, std::string("Thu"));
    CHECK_EQ(groups[1].values.size(), 2); // 15.0 and 18.0
    CHECK_NEAR(groups[1].values[0], 15.0, 1e-9);
    CHECK_NEAR(groups[1].values[1], 18.0, 1e-9);

    CHECK_EQ(groups[2].key, std::string("Sat"));
    CHECK_EQ(groups[2].values.size(), 1); // 30.0
    CHECK_NEAR(groups[2].values[0], 30.0, 1e-9);

    // 3. Test edge cases: empty dataset, single group
    Dataset empty_ds;
    empty_ds.add_text_column("day", {});
    empty_ds.add_numeric_column("bill", {});
    CHECK_EQ(group_by(empty_ds, "day", "bill").size(), 0);

    Dataset single_ds;
    single_ds.add_text_column("day", {"Sun", "Sun"});
    single_ds.add_numeric_column("bill", {10.0, 12.0});
    auto single_groups = group_by(single_ds, "day", "bill");
    CHECK_EQ(single_groups.size(), 1);
    CHECK_EQ(single_groups[0].key, std::string("Sun"));
    CHECK_EQ(single_groups[0].values.size(), 2);

    // 4. Test error handling
    CHECK_THROWS(group_by(ds, "unknown", "bill"), DataError);
    CHECK_THROWS(group_by(ds, "day", "unknown"), DataError);
    CHECK_THROWS(group_by(ds, "bill", "bill"), DataError); // Category column is numeric
    CHECK_THROWS(group_by(ds, "day", "day"), DataError);   // Value column is text

    return finish();
}