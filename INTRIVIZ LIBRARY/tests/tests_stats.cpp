#include "mini_test.hpp"
#include "cppviz/stats.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // --- Normal cases (Hand-computed test values) ---
    CHECK_NEAR(mean({1.0, 2.0, 3.0, 4.0}), 2.5, 1e-9);
    CHECK_NEAR(median({3.0, 1.0, 2.0}), 2.0, 1e-9);
    CHECK_NEAR(variance({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0}), 4.571428571428571, 1e-6);
    CHECK_NEAR(stddev({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0}), 2.138089, 1e-5);

    CHECK_NEAR(min_value({5.0, 1.0, 10.0, -2.0}), -2.0, 1e-9);
    CHECK_NEAR(max_value({5.0, 1.0, 10.0, -2.0}), 10.0, 1e-9);

    CHECK_NEAR(quantile({1.0, 2.0, 3.0, 4.0, 5.0}, 0.25), 2.0, 1e-9);
    CHECK_NEAR(quantile({1.0, 2.0, 3.0, 4.0, 5.0}, 0.75), 4.0, 1e-9);

    // --- Edge cases ---
    CHECK_NEAR(mean({5.0}), 5.0, 1e-9);
    CHECK_NEAR(median({5.0}), 5.0, 1e-9);
    CHECK_NEAR(quantile({5.0}, 0.5), 5.0, 1e-9);

    // --- Invalid inputs ---
    CHECK_THROWS(mean({}), DataError);
    CHECK_THROWS(median({}), DataError);
    CHECK_THROWS(variance({}), DataError);
    CHECK_THROWS(variance({1.0}), DataError);
    CHECK_THROWS(stddev({1.0}), DataError);
    CHECK_THROWS(min_value({}), DataError);
    CHECK_THROWS(max_value({}), DataError);
    CHECK_THROWS(quantile({}, 0.5), DataError);
    CHECK_THROWS(quantile({1.0, 2.0}, -0.1), InvalidArgument);
    CHECK_THROWS(quantile({1.0, 2.0}, 1.1), InvalidArgument);

    return finish();
}