/**
 * @file stats.cpp
 * @brief Implementation of statistical computation and inference algorithms.
 *
 * Implements descriptive statistics (mean, sample variance, standard deviation,
 * extrema, quantiles, median), bivariate statistics (sample covariance, Pearson correlation,
 * correlation matrices), and bootstrap confidence interval estimation.
 */

#include "intriviz/stats.hpp"
#include "intriviz/error.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

namespace intriviz {

// --- Week 1 Implementations ---

/**
 * @brief Computes the arithmetic mean of a vector of numeric values.
 * 
 * @param v Vector of input values.
 * @return The arithmetic mean sum(v) / N.
 * @throws DataError If v is empty.
 */
double mean(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("mean requires at least one value");
    }
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / static_cast<double>(v.size());
}

/**
 * @brief Computes the sample variance (with Bessel's correction, N - 1 denominator).
 * 
 * @param v Vector of input values.
 * @return Sample variance.
 * @throws DataError If v contains fewer than 2 values.
 */
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

/**
 * @brief Computes the sample standard deviation (square root of sample variance).
 * 
 * @param v Vector of input values.
 * @return Sample standard deviation.
 * @throws DataError If v contains fewer than 2 values.
 */
double stddev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

/**
 * @brief Finds the minimum value in a numeric vector.
 * 
 * @param v Vector of values.
 * @return The smallest element in v.
 * @throws DataError If v is empty.
 */
double min_value(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("min_value requires at least one value");
    }
    return *std::min_element(v.begin(), v.end());
}

/**
 * @brief Finds the maximum value in a numeric vector.
 * 
 * @param v Vector of values.
 * @return The largest element in v.
 * @throws DataError If v is empty.
 */
double max_value(const std::vector<double>& v) {
    if (v.empty()) {
        throw DataError("max_value requires at least one value");
    }
    return *std::max_element(v.begin(), v.end());
}

/**
 * @brief Computes the q-th quantile of a vector of values using linear interpolation.
 * 
 * Sorts a copy of the input vector and computes the position index pos = q * (N - 1).
 * 
 * @param v Input vector of values (passed by value to allow local sorting).
 * @param q Quantile fraction between 0.0 and 1.0 (e.g. 0.25, 0.5, 0.75).
 * @return The interpolated quantile value.
 * @throws DataError If v is empty.
 * @throws InvalidArgument If q < 0.0 or q > 1.0.
 */
double quantile(std::vector<double> v, double q) {
    if (v.empty()) {
        throw DataError("quantile requires at least one value");
    }
    if (q < 0.0 || q > 1.0) {
        throw InvalidArgument("quantile q must be in range [0, 1]");
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

/**
 * @brief Computes the sample median (50th percentile) of a numeric vector.
 * 
 * @param v Input vector of values.
 * @return Median value.
 */
double median(std::vector<double> v) {
    return quantile(v, 0.5);
}

// --- Week 2 Implementations ---

/**
 * @brief Computes the sample covariance between two equal-length numeric vectors.
 * 
 * Uses Bessel's correction (N - 1 denominator).
 * 
 * @param a First data vector.
 * @param b Second data vector.
 * @return Sample covariance.
 * @throws InvalidArgument If vectors have different sizes.
 * @throws DataError If vectors contain fewer than 2 elements.
 */
double covariance(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw InvalidArgument("covariance vectors must have the same size");
    }
    if (a.size() < 2) {
        throw DataError("covariance requires at least two values");
    }

    double mean_a = mean(a);
    double mean_b = mean(b);
    double accum = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        accum += (a[i] - mean_a) * (b[i] - mean_b);
    }
    return accum / static_cast<double>(a.size() - 1);
}

/**
 * @brief Computes Pearson's correlation coefficient r between two vectors: cov(a, b) / (stddev(a) * stddev(b)).
 * 
 * @param a First data vector.
 * @param b Second data vector.
 * @return Pearson correlation coefficient r in range [-1.0, 1.0].
 * @throws InvalidArgument If vectors have different sizes.
 * @throws DataError If vectors contain fewer than 2 elements or standard deviation is zero.
 */
double correlation(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw InvalidArgument("correlation vectors must have the same size");
    }
    if (a.size() < 2) {
        throw DataError("correlation requires at least two values");
    }

    double sa = stddev(a);
    double sb = stddev(b);
    if (sa == 0.0 || sb == 0.0) {
        throw DataError("correlation is undefined for constant vectors (zero standard deviation)");
    }

    return covariance(a, b) / (sa * sb);
}

/**
 * @brief Computes a symmetric N x N Pearson correlation matrix for specified numeric columns in a Dataset.
 * 
 * Diagonal elements are 1.0, and off-diagonal elements [i][j] equal correlation(col_i, col_j).
 * 
 * @param data Source Dataset.
 * @param numeric_columns List of numeric column names to analyze.
 * @return Square matrix of correlation coefficients.
 * @throws InvalidArgument If numeric_columns is empty.
 */
std::vector<std::vector<double>> correlation_matrix(const Dataset& data,
                                                    const std::vector<std::string>& numeric_columns) {
    if (numeric_columns.empty()) {
        throw InvalidArgument("correlation_matrix requires at least one column");
    }

    std::size_t n = numeric_columns.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0.0));

    for (std::size_t i = 0; i < n; ++i) {
        matrix[i][i] = 1.0;
        for (std::size_t j = i + 1; j < n; ++j) {
            const auto& col_i = data.numeric(numeric_columns[i]);
            const auto& col_j = data.numeric(numeric_columns[j]);
            double corr = correlation(col_i, col_j);
            matrix[i][j] = corr;
            matrix[j][i] = corr;
        }
    }

    return matrix;
}

/**
 * @brief Computes a non-parametric bootstrap confidence interval for the population mean.
 * 
 * Resamples the input vector with replacement `n_boot` times using Mersenne Twister (std::mt19937),
 * evaluates the sample mean on each replicate, and computes empirical percentile bounds.
 * 
 * @param v Vector of observation values.
 * @param level Confidence level strictly between 0.0 and 1.0 (e.g., 0.95 for 95% CI).
 * @param n_boot Number of bootstrap replications to generate (e.g., 1000).
 * @param seed Pseudo-random generator seed for reproducible resampling.
 * @return Interval struct containing lower and upper confidence limits {low, high}.
 * @throws DataError If v is empty.
 * @throws InvalidArgument If level <= 0 or level >= 1, or n_boot <= 0.
 */
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

    std::vector<double> boot_means;
    boot_means.reserve(static_cast<std::size_t>(n_boot));

    for (int b = 0; b < n_boot; ++b) {
        double sum = 0.0;
        for (std::size_t i = 0; i < v.size(); ++i) {
            sum += v[dist(rng)];
        }
        boot_means.push_back(sum / static_cast<double>(v.size()));
    }

    double alpha = (1.0 - level) / 2.0;
    double low_q = alpha;
    double high_q = 1.0 - alpha;

    return Interval{quantile(boot_means, low_q), quantile(boot_means, high_q)};
}

} // namespace intriviz
