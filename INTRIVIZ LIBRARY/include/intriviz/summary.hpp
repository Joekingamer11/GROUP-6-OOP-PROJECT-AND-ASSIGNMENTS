 #pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "intriviz/dataset.hpp"

namespace intriviz {

/// Contains summary statistics for a single numeric column (excluding missing values).
struct ColumnSummary {
    std::string name;
    std::size_t count = 0;                 // Number of non-missing values
    double mean = 0.0;                     // Arithmetic mean
    double stddev = 0.0;                   // Sample standard deviation (0.0 if count < 2)
    double min = 0.0;                      // Minimum value
    double q25 = 0.0;                      // 25th percentile (1st quartile)
    double median = 0.0;                   // Median (50th percentile)
    double q75 = 0.0;                      // 75th percentile (3rd quartile)
    double max = 0.0;                      // Maximum value
};

/// Computes summary statistics for all numeric columns in a Dataset.
/// Skips text columns and ignores missing values (NaN).
/// @param data Input Dataset table.
/// @return List of ColumnSummary structures.

std::vector<ColumnSummary> describe(const Dataset& data);
/// Formats column summary structures into a neatly aligned ASCII text table.
/// @param summaries List of column summaries.
/// @return Formatted string containing the formatted ASCII table.
/// Formats a list of column summaries into a neatly aligned ASCII text table [1, 3].
std::string to_string(const std::vector<ColumnSummary>& summaries);

} 