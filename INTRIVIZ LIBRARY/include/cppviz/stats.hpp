#pragma once

#include <string>
#include <vector>
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

namespace cppviz {

// Interval struct used for confidence intervals
struct Interval {
    double low = 0.0;
    double high = 0.0;
};

// --- Week 1 API ---
double mean(const std::vector<double>& v);
double variance(const std::vector<double>& v);
double stddev(const std::vector<double>& v);
double min_value(const std::vector<double>& v);
double max_value(const std::vector<double>& v);
double quantile(std::vector<double> v, double q);
double median(std::vector<double> v);

// --- Week 2 API ---
double covariance(const std::vector<double>& a, const std::vector<double>& b);
double correlation(const std::vector<double>& a, const std::vector<double>& b);
std::vector<std::vector<double>> correlation_matrix(const Dataset& data,
                                                    const std::vector<std::string>& numeric_columns);

// --- Week 3 Performance-Optimized Bootstrap API ---
// Computes percentile bootstrap confidence interval for the mean.
// Resamples with replacement n_boot times using std::mt19937 and seed.
// Optimized in Week 3 with memory pre-allocation to support fast pointplot rendering.
Interval bootstrap_ci(const std::vector<double>& v, double level = 0.95,
                     int n_boot = 1000, unsigned seed = 42);

} // namespace cppviz