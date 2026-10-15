//ANNA KAGEMULO MTABAZI 25/T/08565/PS   
//THIS code contains the implementation of the summary statistics functions for the C++ Visualization Library (cppviz).
//The functions provided in this file compute descriptive statistics for numeric columns in a dataset and format the results

#include "cppviz/summary.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/stats.hpp"

#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

namespace cppviz {

std::vector<ColumnSummary> describe(const Dataset& data) {
    std::vector<ColumnSummary> summaries;
    std::vector<std::string> col_names = data.column_names();

    for (const auto& name : col_names) {
        // Automatically skip non-numeric (text) columns
        if (!data.is_numeric(name)) {
            continue;
        }

        const auto& col_values = data.numeric(name);

        // Filter out missing values (NaN)
        std::vector<double> valid_values;
        valid_values.reserve(col_values.size());
        for (double val : col_values) {
            if (!is_missing(val)) {
                valid_values.push_back(val);
            }
        }

        ColumnSummary summary;
        summary.name = name;
        summary.count = valid_values.size();

        if (valid_values.empty()) {
            // Default 0.0 values if no valid observations exist
            summary.mean = 0.0;
            summary.stddev = 0.0;
            summary.min = 0.0;
            summary.q25 = 0.0;
            summary.median = 0.0;
            summary.q75 = 0.0;
            summary.max = 0.0;
        } else {
            // Compute descriptive statistics using stats.hpp (M5)
            summary.mean = mean(valid_values);
            summary.min = min_value(valid_values);
            summary.max = max_value(valid_values);
            summary.q25 = quantile(valid_values, 0.25);
            summary.median = median(valid_values);
            summary.q75 = quantile(valid_values, 0.75);

            // Avoid division-by-zero runtime error if sample size < 2
            if (valid_values.size() >= 2) {
                summary.stddev = stddev(valid_values);
            } else {
                summary.stddev = 0.0;
            }
        }

        summaries.push_back(summary);
    }

    return summaries;
}

std::string to_string(const std::vector<ColumnSummary>& summaries) {
    if (summaries.empty()) {
        return "Dataset Summary: (no numeric columns)\n";
    }

    std::ostringstream oss;
    const int name_width = 16;
    const int num_width = 12;

    // Table Header
    oss << std::left << std::setw(name_width) << "Column"
        << std::right << std::setw(8) << "Count"
        << std::setw(num_width) << "Mean"
        << std::setw(num_width) << "StdDev"
        << std::setw(num_width) << "Min"
        << std::setw(num_width) << "25%"
        << std::setw(num_width) << "Median"
        << std::setw(num_width) << "75%"
        << std::setw(num_width) << "Max" << "\n";

    // Separator Line
    oss << std::string(name_width + 8 + num_width * 7, '-') << "\n";

    // Table Rows (Formatted to 2 decimal places)
    oss << std::fixed << std::setprecision(2);

    for (const auto& s : summaries) {
        oss << std::left << std::setw(name_width) << s.name
            << std::right << std::setw(8) << s.count
            << std::setw(num_width) << s.mean
            << std::setw(num_width) << s.stddev
            << std::setw(num_width) << s.min
            << std::setw(num_width) << s.q25
            << std::setw(num_width) << s.median
            << std::setw(num_width) << s.q75
            << std::setw(num_width) << s.max << "\n";
    }

    return oss.str();
}

}
 // namespace cppviz