/**
 * @file preprocess.cpp
 * @brief Implementation of data cleaning, imputation, filtering, and normalization operations.
 *
 * Provides data transformation pipelines for Dataset instances and numeric vectors,
 * including row-wise missing value removal, constant imputation, row filtering,
 * column projection, Min-Max scaling, and Z-score standardization.
 */

#include "intriviz/preprocess.hpp"
#include "intriviz/error.hpp"
#include "intriviz/stats.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace intriviz {

/**
 * @brief Filters out any row that contains missing values (NaN or empty text strings).
 * 
 * Inspects all columns across each row. Rows with any missing observation are omitted.
 * 
 * @param data Input Dataset.
 * @return A new Dataset containing only complete rows.
 */
Dataset drop_missing(const Dataset& data) {
    std::size_t num_rows = data.row_count();
    auto col_names = data.column_names();
    std::vector<std::size_t> keep_indices;

    for (std::size_t r = 0; r < num_rows; ++r) {
        bool missing = false;
        for (const auto& name : col_names) {
            if (data.is_numeric(name)) {
                double val = data.numeric(name)[r];
                if (is_missing(val) || std::isnan(val)) {
                    missing = true;
                    break;
                }
            } else {
                const std::string& val = data.text(name)[r];
                if (val.empty()) {
                    missing = true;
                    break;
                }
            }
        }
        if (!missing) {
            keep_indices.push_back(r);
        }
    }
    return data.select_rows(keep_indices);
}

/**
 * @brief Replaces missing (NaN) values in a specific numeric column with a constant replacement value.
 * 
 * @param data Input Dataset.
 * @param column Name of the numeric column to impute.
 * @param value Constant value to substitute in place of NaN.
 * @return A new Dataset with missing values in the specified column replaced.
 * @throws DataError If column does not exist or is not numeric.
 */
Dataset fill_missing(const Dataset& data, const std::string& column, double value) {
    if (!data.has_column(column)) {
        throw DataError("fill_missing: column '" + column + "' not found");
    }
    if (!data.is_numeric(column)) {
        throw DataError("fill_missing: column '" + column + "' is not numeric");
    }

    Dataset result;
    for (const auto& name : data.column_names()) {
        if (data.is_numeric(name)) {
            auto vec = data.numeric(name);
            if (name == column) {
                for (auto& v : vec) {
                    if (is_missing(v) || std::isnan(v)) {
                        v = value;
                    }
                }
            }
            result.add_numeric_column(name, vec);
        } else {
            result.add_text_column(name, data.text(name));
        }
    }
    return result;
}

/**
 * @brief Filters rows based on a custom predicate evaluated on a numeric column.
 * 
 * Rows with NaN values in the specified column are automatically excluded.
 * 
 * @param data Input Dataset.
 * @param numeric_column Column name whose values will be passed to the predicate.
 * @param keep Unary predicate returning true for rows to retain.
 * @return Filtered Dataset containing matching rows.
 * @throws DataError If numeric_column is missing or not numeric.
 */
Dataset filter_rows(const Dataset& data, const std::string& numeric_column,
                    const std::function<bool(double)>& keep) {
    if (!data.has_column(numeric_column)) {
        throw DataError("filter_rows: column '" + numeric_column + "' not found");
    }
    if (!data.is_numeric(numeric_column)) {
        throw DataError("filter_rows: column '" + numeric_column + "' is not numeric");
    }

    const auto& values = data.numeric(numeric_column);
    std::vector<std::size_t> keep_indices;
    for (std::size_t i = 0; i < values.size(); ++i) {
        double val = values[i];
        if (!is_missing(val) && !std::isnan(val) && keep(val)) {
            keep_indices.push_back(i);
        }
    }
    return data.select_rows(keep_indices);
}

/**
 * @brief Projects a subset of columns into a new Dataset in the specified order.
 * 
 * @param data Input Dataset.
 * @param names Ordered list of column identifiers to include.
 * @return Projected Dataset.
 * @throws DataError If any requested column name does not exist in data.
 */
Dataset select_columns(const Dataset& data, const std::vector<std::string>& names) {
    Dataset result;
    for (const auto& name : names) {
        if (!data.has_column(name)) {
            throw DataError("select_columns: column '" + name + "' not found");
        }
        if (data.is_numeric(name)) {
            result.add_numeric_column(name, data.numeric(name));
        } else {
            result.add_text_column(name, data.text(name));
        }
    }
    return result;
}

/**
 * @brief Normalizes a vector of numeric values to the range [0.0, 1.0] using Min-Max scaling.
 * 
 * Formula: (x - min) / (max - min). NaN values in the input are preserved as NaN.
 * 
 * @param values Input vector of values.
 * @return Vector of normalized values in [0.0, 1.0].
 * @throws DataError If values is empty, contains only NaNs, or all values are identical (max == min).
 */
std::vector<double> normalize_minmax(const std::vector<double>& values) {
    if (values.empty()) {
        throw DataError("normalize_minmax: input vector is empty");
    }

    double min_val = std::numeric_limits<double>::infinity();
    double max_val = -std::numeric_limits<double>::infinity();
    bool has_valid = false;

    for (double v : values) {
        if (!is_missing(v) && !std::isnan(v)) {
            if (v < min_val) min_val = v;
            if (v > max_val) max_val = v;
            has_valid = true;
        }
    }

    if (!has_valid || min_val == max_val) {
        throw DataError("normalize_minmax: all values are equal or missing");
    }

    std::vector<double> result;
    result.reserve(values.size());
    double range = max_val - min_val;

    for (double v : values) {
        if (is_missing(v) || std::isnan(v)) {
            result.push_back(std::numeric_limits<double>::quiet_NaN());
        } else {
            result.push_back((v - min_val) / range);
        }
    }
    return result;
}

/**
 * @brief Computes Z-score standardization for a vector of values: (x - mean) / stddev.
 * 
 * Centers data to have a mean of 0.0 and a standard deviation of 1.0. NaN values are preserved.
 * 
 * @param values Input vector of values.
 * @return Vector of standardized values.
 * @throws DataError If values is empty or standard deviation is 0.0 (constant values).
 */
std::vector<double> standardize(const std::vector<double>& values) {
    if (values.empty()) {
        throw DataError("standardize: input vector is empty");
    }

    // Reuse mean() and stddev() from stats.hpp per project specification
    double m = mean(values);
    double s = stddev(values);

    if (s == 0.0) {
        throw DataError("standardize: standard deviation is zero (all values equal)");
    }

    std::vector<double> result;
    result.reserve(values.size());

    for (double v : values) {
        if (is_missing(v) || std::isnan(v)) {
            result.push_back(std::numeric_limits<double>::quiet_NaN());
        } else {
            result.push_back((v - m) / s);
        }
    }
    return result;
}

} // namespace intriviz
