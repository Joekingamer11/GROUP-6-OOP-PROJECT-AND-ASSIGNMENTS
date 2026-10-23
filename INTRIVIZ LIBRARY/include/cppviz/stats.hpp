#pragma once

#include <string>
#include <vector>
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

namespace cppviz {

// Interval struct used for confidence intervals (e.g. in pointplot)
struct Interval {
    double low = 0.0;
    double high = 0.0;
};

// --- Week 1 API ---

// Returns the arithmetic mean. Throws DataError if the vector is empty.
double mean(const std::vector<double>& v);

// Returns sample variance (divides by n - 1). Throws DataError if size < 2.
double variance(const std::vector<double>& v);

// Returns standard deviation. Throws DataError if size < 2.
double stddev(const std::vector<double>& v);

// Returns the minimum value. Throws DataError if empty.
double min_value(const std::vector<double>& v);

// Returns the maximum value. Throws DataError if empty.
double max_value(const std::vector<double>& v);

// Returns q-th quantile (q in [0, 1]) via linear interpolation.
// Throws DataError if empty, or InvalidArgument if q is out of range.
double quantile(std::vector<double> v, double q);

// Returns median value by computing quantile(v, 0.5).
double median(std::vector<double> v);

// --- Week 2 Extensions ---

// Computes sample covariance between two equal-sized vectors.
// Throws InvalidArgument if vector lengths differ, or DataError if size < 2.
double covariance(const std::vector<double>& a, const std::vector<double>& b);

// Computes Pearson correlation coefficient (-1.0 to 1.0).
// Throws InvalidArgument if lengths differ, or DataError if size < 2 or
// if either series is constant (zero variance).
double correlation(const std::vector<double>& a, const std::vector<double>& b);

// Computes an n x n correlation matrix for the specified numeric columns in a Dataset.
// Diagonal entries are set to 1.0. Throws InvalidArgument if column list is empty,
// or DataError if a requested column is missing/non-numeric.
std::vector<std::vector<double>> correlation_matrix(const Dataset& data,
                                                    const std::vector<std::string>& numeric_columns);

// Computes a percentile bootstrap confidence interval for the mean.
// Resamples with replacement n_boot times using std::mt19937 and the given seed.
// Throws DataError if empty, or InvalidArgument if level is not in (0, 1) or n_boot <= 0.
Interval bootstrap_ci(const std::vector<double>& v, double level = 0.95,
                     int n_boot = 1000, unsigned seed = 42);

} // namespace cppviz