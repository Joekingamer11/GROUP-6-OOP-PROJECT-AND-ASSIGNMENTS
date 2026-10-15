#pragma once

#include <vector>
#include "cppviz/error.hpp"

namespace cppviz {

// Interval struct used for confidence intervals (extended in Week 2)
struct Interval {
    double low = 0.0;
    double high = 0.0;
};

// --- Week 1 Public API ---

// Returns the arithmetic mean. Throws DataError if the input vector is empty.
double mean(const std::vector<double>& v);

// Returns the sample variance (divides by n - 1). Throws DataError if size < 2.
double variance(const std::vector<double>& v);

// Returns standard deviation. Throws DataError if size < 2.
double stddev(const std::vector<double>& v);

// Returns the minimum value in the vector. Throws DataError if empty.
double min_value(const std::vector<double>& v);

// Returns the maximum value in the vector. Throws DataError if empty.
double max_value(const std::vector<double>& v);

// Returns the q-th quantile (q in [0, 1]) using linear interpolation.
// Throws DataError if empty, or InvalidArgument if q is out of range [0, 1].
double quantile(std::vector<double> v, double q);

// Returns the median value by taking the vector by value and computing quantile(v, 0.5).
double median(std::vector<double> v);

} // namespace cppviz