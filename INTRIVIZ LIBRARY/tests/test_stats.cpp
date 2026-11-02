#include "mini_test.hpp"
#include "intriviz/stats.hpp"
#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"

int main() {
    using namespace intriviz;

    // --- Week 1 Tests ---
    CHECK_NEAR(mean({1.0, 2.0, 3.0, 4.0}), 2.5, 1e-9);
    CHECK_NEAR(median({3.0, 1.0, 2.0}), 2.0, 1e-9);
    CHECK_NEAR(variance({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0}), 4.571428571428571, 1e-6);
    CHECK_NEAR(stddev({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0}), 2.138090, 1e-5);
    
    CHECK_NEAR(min_value({5.0, 1.0, 10.0, -2.0}), -2.0, 1e-9);
    CHECK_NEAR(max_value({5.0, 1.0, 10.0, -2.0}), 10.0, 1e-9);
    
    CHECK_NEAR(quantile({1.0, 2.0, 3.0, 4.0, 5.0}, 0.25), 2.0, 1e-9);
    CHECK_NEAR(quantile({1.0, 2.0, 3.0, 4.0, 5.0}, 0.75), 4.0, 1e-9);

    // --- Week 2 Covariance & Correlation Tests ---
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y_pos = {2.0, 4.0, 6.0, 8.0, 10.0};
    std::vector<double> y_neg = {5.0, 4.0, 3.0, 2.0, 1.0};

    CHECK_NEAR(covariance(x, y_pos), 5.0, 1e-9);
    CHECK_NEAR(correlation(x, y_pos), 1.0, 1e-9);
    CHECK_NEAR(correlation(x, y_neg), -1.0, 1e-9);

    // --- Correlation Matrix Test ---
    Dataset ds;
    ds.add_numeric_column("x", x);
    ds.add_numeric_column("y_pos", y_pos);
    ds.add_numeric_column("y_neg", y_neg);

    auto matrix = correlation_matrix(ds, {"x", "y_pos", "y_neg"});
    CHECK_EQ(matrix.size(), 3);
    CHECK_NEAR(matrix[0][0], 1.0, 1e-9);
    CHECK_NEAR(matrix[0][1], 1.0, 1e-9);
    CHECK_NEAR(matrix[0][2], -1.0, 1e-9);
    CHECK_NEAR(matrix[1][2], -1.0, 1e-9);

    // --- Bootstrap Confidence Interval Tests ---
    std::vector<double> const_vec = {5.0, 5.0, 5.0, 5.0};
    Interval ci_const = bootstrap_ci(const_vec, 0.95, 100, 42);
    CHECK_NEAR(ci_const.low, 5.0, 1e-9);
    CHECK_NEAR(ci_const.high, 5.0, 1e-9);

    std::vector<double> sample = {10.0, 12.0, 11.0, 15.0, 14.0, 13.0};
    Interval ci = bootstrap_ci(sample, 0.95, 1000, 42);
    double sample_mean = mean(sample);
    CHECK(ci.low <= sample_mean);
    CHECK(ci.high >= sample_mean);

    // --- Edge Cases ---
    Interval single_ci = bootstrap_ci({7.5}, 0.95, 100, 42);
    CHECK_NEAR(single_ci.low, 7.5, 1e-9);
    CHECK_NEAR(single_ci.high, 7.5, 1e-9);

    // --- Invalid Inputs & Exceptions ---
    {
        bool threw = false;
        try { covariance({1.0, 2.0}, {1.0}); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { correlation({1.0, 2.0}, {1.0}); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { covariance({1.0}, {2.0}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { correlation({1.0}, {2.0}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // Constant vector correlation (zero standard deviation)
    {
        bool threw = false;
        try { correlation({1.0, 2.0, 3.0}, {2.0, 2.0, 2.0}); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    // Invalid bootstrap arguments
    {
        bool threw = false;
        try { bootstrap_ci({}, 0.95); } catch (const DataError&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { bootstrap_ci({1.0, 2.0}, 1.5); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }
    {
        bool threw = false;
        try { bootstrap_ci({1.0, 2.0}, 0.95, 0); } catch (const InvalidArgument&) { threw = true; } catch (...) {}
        CHECK(threw);
    }

    return finish();
}