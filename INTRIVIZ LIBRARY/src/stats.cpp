#include "cppviz/stats.hpp"
#include "cppviz/error.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

namespace cppviz {

// ... [Existing Week 1 & 2 Functions: mean, variance, stddev, min_value, max_value, quantile, median, covariance, correlation, correlation_matrix] ...

// --- Week 3 Performance Optimization for bootstrap_ci ---
Interval bootstrap_ci(const std::vector<double>& v, double level, int n_boot, unsigned seed) {
    if (v.empty()) {
        throw DataError("bootstrap_ci requires at least one value");
    }
    if (level <= 0.0 || level >= 1.0) {
        throw InvalidArgument("bootstrap confidence level must be strictly between 0 and 1");
    }
    if (n_boot <= 0) {
        throw InvalidArgument("n_boot must be positive");
    }

    // Single-element edge case: interval collapses to point value
    if (v.size() == 1) {
        return Interval{v[0], v[0]};
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::size_t> dist(0, v.size() - 1);

    // Week 3 Optimization: Pre-allocate memory buffer for boot_means
    std::vector<double> boot_means;
    boot_means.reserve(static_cast<std::size_t>(n_boot));

    const double inv_n = 1.0 / static_cast<double>(v.size());

    for (int b = 0; b < n_boot; ++b) {
        double sum = 0.0;
        for (std::size_t i = 0; i < v.size(); ++i) {
            sum += v[dist(rng)];
        }
        boot_means.push_back(sum * inv_n);
    }

    double alpha = (1.0 - level) / 2.0;
    double low_q = alpha;
    double high_q = 1.0 - alpha;

    return Interval{quantile(boot_means, low_q), quantile(boot_means, high_q)};
}

} // namespace cppviz