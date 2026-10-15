#include "cppviz/stats.hpp"
#include "cppviz/error.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace cppviz {

double mean(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("mean requires at least one value");
    }
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / static_cast<double>(v.size());
}

double variance(const std::vector<double>& v) {
    if (v.size() < 2) {
        throw DataError("variance requires at least two values");
    }
    double m = mean(v);
    double accum = 0.0;
    for (double x : v) {
        accum += (x - m) * (x - m);
    }
    return accum / static_cast<double>(v.size() - 1);
}

double stddev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

double min_value(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("min_value requires at least one value");
    }
    return *std::min_element(v.begin(), v.end());
}

double max_value(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("max_value requires at least one value");
    }
    return *std::max_element(v.begin(), v.end());
}

double quantile(std::vector<double> v, double q) {
    if (v.empty()) {
        throw DataError("quantile requires at least one value");
    }
    // Corrected the string literal from [6] to [0,1] to match the logic
    if (q < 0.0 || q > 1.0) {
        throw InvalidArgument("quantile q must be in range [0,1]");
    }
    if (v.size() == 1) {
        return v[0];
    }

    std::sort(v.begin(), v.end());
    double pos = q * static_cast<double>(v.size() - 1);
    std::size_t idx = static_cast<std::size_t>(pos);
    double frac = pos - static_cast<double>(idx);

    if (idx >= v.size() - 1) {
        return v.back();
    }
    return v[idx] + frac * (v[idx + 1] - v[idx]);
}

double median(std::vector<double> v) {
    // Reuses quantile calculation
    return quantile(v, 0.5);
}

} // namespace cppviz