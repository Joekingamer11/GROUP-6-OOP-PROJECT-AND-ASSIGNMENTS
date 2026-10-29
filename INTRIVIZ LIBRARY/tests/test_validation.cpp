#include "mini_test.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"
#include "intriviz/validation.hpp"

int main() {
    using namespace intriviz;

    // Create test dataset
    Dataset data;
    data.add_numeric_column("bill", {10.5, 20.0, 15.2});
    data.add_text_column("day", {"Mon", "Tue", "Wed"});

    // 1. Test require_column
    require_column(data, "bill");
    require_column(data, "day");
    CHECK_THROWS(require_column(data, "missing_col"), DataError);

    // 2. Test require_numeric_column
    require_numeric_column(data, "bill");
    CHECK_THROWS(require_numeric_column(data, "day"), DataError);
    CHECK_THROWS(require_numeric_column(data, "unknown"), DataError);

    // 3. Test require_text_column
    require_text_column(data, "day");
    CHECK_THROWS(require_text_column(data, "bill"), DataError);
    CHECK_THROWS(require_text_column(data, "unknown"), DataError);

    // 4. Test require_not_empty
    require_not_empty(data);
    Dataset empty_data;
    CHECK_THROWS(require_not_empty(empty_data), DataError);

    // 5. Test require_same_size
    require_same_size(3, 3, "vector comparison");
    CHECK_THROWS(require_same_size(3, 5, "vector comparison"), InvalidArgument);

    return finish();
}