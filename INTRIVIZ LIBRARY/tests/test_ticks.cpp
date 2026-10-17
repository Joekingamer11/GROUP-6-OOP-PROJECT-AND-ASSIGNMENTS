#include "mini_test.hpp"
#include "cppviz/ticks.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    //Nice_ticks on standard range
    auto ticks = nice_ticks(0, 50, 5);
    CHECK_EQ(ticks.size(), 6u);
    CHECK_NEAR(ticks[0], 0, 1e-9);
    CHECK_NEAR(ticks[5 ], 50, 1e-9);

    //On fractional domain
    auto frac_ticks = nice_ticks(0.15, 0.85, 5);
    CHECK(frac_ticks.size(), >= 2u);
    CHECK(frac_ticks.front(), <= 0.15);
    CHECK(frac_ticks.back(), >= 0.85);

    //Log_ticks powers of 10
    auto l_ticks = log_ticks(0.1, 1000);
    CHECK_EQ(l_ticks.size(), 5u);
    CHECK_NEAR(l_ticks[0], 0.1, 1e-9);
    CHECK_NEAR(l_ticks[4], 1000, 1e-9);

    //format_tick tests
    CHECK_EQ(format_tick(20), "20");
    CHECK_EQ(format_tick(2.5), "2.5");
    CHECK_EQ(format_tick(0.0), "0.0");

    //padded_range normal and constant values
    auto pad = padded_range({10.0, 20.0,}, 0.05);
    CHECK_NEAR(pad.first, 9.5, 1e-9);
    CHECK_NEAR(pad.second, 20.5, 1e-9);

    auto pad_const = padded_range({5.0, 5.0, 5.0}, 0.1);
    CHECK_NEAR(pad_const.first, 4.5, 1e-9);
    CHECK_NEAR(pad_const.second, 5.5, 1e-9);    

    //invalid inputs
    CHECK_THROWS(nice_ticks(10, 5, 5), InvalidArgument);
    CHECK_THROWS(log_ticks(-1.0, 10.0), InvalidArgument);
    CHECK_THROWS(padded_range({}, 0.1), DataError);

    return 0;
}