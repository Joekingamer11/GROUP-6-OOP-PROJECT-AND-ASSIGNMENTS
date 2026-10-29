#pragma once

#include <functional>
#include <string>
#include <vector>
#include "intriviz/dataset.hpp"

namespace intriviz {

// Removes every row from the dataset that contains any NaN (in numeric columns) or empty string (in text columns)
Dataset drop_missing(const Dataset& data);

// Replaces missing (NaN) values in a specific numeric column with a specified default value
Dataset fill_missing(const Dataset& data, const std::string& column, double value);

// Keeps rows where the given predicate function evaluates to true for values in the specified numeric column
Dataset filter_rows(const Dataset& data, const std::string& numeric_column,
                    const std::function<bool(double)>& keep);

// Creates a new Dataset containing only the specified column names
Dataset select_columns(const Dataset& data, const std::vector<std::string>& names);

// Rescales a numeric vector to the range [0, 1]. Throws DataError if the vector is empty or all values are equal.
std::vector<double> normalize_minmax(const std::vector<double>& values);

// Standardizes a numeric vector using z-score: (x - mean) / stddev. Reuses mean() and stddev() from stats.hpp.
std::vector<double> standardize(const std::vector<double>& values);

} 