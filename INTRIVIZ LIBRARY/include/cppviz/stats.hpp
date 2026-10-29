#pragma once

#include <string>
#include <vector>

#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

namespace cppviz {

/// \brief Represents a two-sided confidence interval or range.
struct Interval {
    double low = 0.0;   ///< Lower bound
    double high = 0.0;  ///< Upper bound
};

// --- Univariate Statistics ---

/// \brief Calculates the arithmetic mean of a numeric series.
/// \param v Input vector of values.
/// \return Arithmetic mean value.
/// \throws DataError if input vector is empty.
double mean(const std::vector<double>& v);

/// \brief Calculates the sample variance (divides by n - 1).
/// \param v Input vector of values.
/// \return Sample variance value.
/// \throws DataError if vector contains fewer than 2 elements.
double variance(const std::vector<double>& v);

/// \brief Calculates the sample standard deviation.
/// \param v Input vector of values.
/// \return Sample standard deviation value.
/// \throws DataError if vector contains fewer than 2 elements.
double stddev(const std::vector<double>& v);

/// \brief Finds the minimum numeric value in a vector.
/// \param v Input vector of values.
/// \return Minimum value.
/// \throws DataError if vector is empty.
double min_value(const std::vector<double>& v);

/// \brief Finds the maximum numeric value in a vector.
/// \param v Input vector of values.
/// \return Maximum value.
/// \throws DataError if vector is empty.
double max_value(const std::vector<double>& v);

/// \brief Computes the q-th quantile (q in [0, 1]) using linear interpolation.
/// \param v Input vector of values (passed by value for internal sorting).
/// \param q Quantile position in [0.0, 1.0].
/// \return Interpolated quantile value.
/// \throws DataError if vector is empty.
/// \throws InvalidArgument if q is outside [0, 1].
double quantile(std::vector<double> v, double q);

/// \brief Calculates the median (50th percentile) value of a series.
/// \param v Input vector of values.
/// \return Median value.
/// \throws DataError if vector is empty.
double median(std::vector<double> v);

// --- Bivariate & Multivariate Statistics ---

/// \brief Calculates sample covariance between two equal-sized series.
/// \param a First numeric series.
/// \param b Second numeric series.
/// \return Sample covariance value.
/// \throws InvalidArgument if series lengths differ.
/// \throws DataError if series length is less than 2.
double covariance(const std::vector<double>& a, const std::vector<double>& b);

/// \brief Calculates the Pearson correlation coefficient r in [-1, 1].
/// \param a First numeric series.
/// \param b Second numeric series.
/// \return Pearson correlation coefficient.
/// \throws InvalidArgument if series lengths differ.
/// \throws DataError if series length < 2 or if either series has zero variance.
double correlation(const std::vector<double>& a, const std::vector<double>& b);

/// \brief Generates an n x n symmetric Pearson correlation matrix for specified dataset columns.
/// \param data Tabular dataset containing columns.
/// \param numeric_columns Vector of numeric column names to analyze.
/// \return Square 2D vector matrix where cell (i, j) is correlation(col_i, col_j).
/// \throws InvalidArgument if numeric_columns is empty.
/// \throws DataError if a column is missing or non-numeric.
std::vector<std::vector<double>> correlation_matrix(const Dataset& data,
                                                    const std::vector<std::string>& numeric_columns);

/// \brief Computes a percentile bootstrap confidence interval for the mean.
/// \param v Input numeric values.
/// \param level Confidence level in (0, 1), default 0.95 (95%).
/// \param n_boot Number of Monte Carlo bootstrap resamples, default 1000.
/// \param seed Fixed Random seed for deterministic testing, default 42.
/// \return Interval containing low and high confidence bounds.
/// \throws DataError if vector is empty.
/// \throws InvalidArgument if level is not in (0, 1) or n_boot <= 0.
Interval bootstrap_ci(const std::vector<double>& v, double level = 0.95,
                     int n_boot = 1000, unsigned seed = 42);

} // namespace cppviz
