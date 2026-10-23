#include "mini_test.hpp"
#include "cppviz/csv_loader.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // 1. Normal 3-column CSV string
    std::string csv_data = "name,age,score\nAlice,20,88.5\nBob,22,91.0";
    Dataset ds = parse_csv(csv_data);
    CHECK_EQ(ds.row_count(), 2);
    CHECK_EQ(ds.column_count(), 3);

    // 2. Empty cell converted to NaN
    std::string missing_data = "x,y\n1.0,\n2.0,3.0";
    Dataset ds_missing = parse_csv(missing_data);
    const auto& y_values = ds_missing.numeric("y");
    CHECK(is_missing(y_values[0]));
    CHECK(!is_missing(y_values[1]));

    // 3. Quoted fields containing commas
    std::string quoted_csv = "id,name\n1,\"Smith, John\"";
    Dataset ds_quoted = parse_csv(quoted_csv);
    CHECK_EQ(ds_quoted.text("name")[0], std::string("Smith, John"));

    // 4. Invalid input: Ragged row throws DataError
    std::string ragged_csv = "a,b\n1,2,3";
    CHECK_THROWS(parse_csv(ragged_csv), DataError);

    // 5. Non-existent file throws IoError
    CHECK_THROWS(load_csv("nonexistent_file.csv"), IoError);

    return finish();
}