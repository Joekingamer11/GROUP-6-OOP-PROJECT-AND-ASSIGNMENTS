/*
ANNA KAGEMULO MTABAZI 25/T/08565/PS
Summary of the C++ Visualization Library
This header file provides a summary of the C++ Visualization Library (cppviz). 
It includes essential components and functionalities that facilitate the visualization of data structures and algorithms in C++.
The library is designed to be compatible with the C++14 standard, ensuring modern features and optimizations are available for developers.
 */

 #pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "cppviz/dataset.hpp"

namespace cppviz {

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

/// Computes descriptive statistics for every numeric column in the dataset.
/// Skips text columns and ignores missing values (NaN) [1, 3].
std::vector<ColumnSummary> describe(const Dataset& data);

/// Formats a list of column summaries into a neatly aligned ASCII text table [1, 3].
std::string to_string(const std::vector<ColumnSummary>& summaries);

} 