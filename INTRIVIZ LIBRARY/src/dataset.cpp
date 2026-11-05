/**
 * @file dataset.cpp
 * @brief Implementation of in-memory columnar tabular data storage (Dataset).
 *
 * Dataset provides a column-oriented data frame holding typed numeric (std::vector<double>)
 * and categorical/text (std::vector<std::string>) columns with missing-value handling,
 * validation, and row filtering/slicing capabilities.
 */

#include "intriviz/dataset.hpp"
#include "intriviz/error.hpp"

#include <cmath>
#include <utility>

namespace intriviz {

/**
 * @brief Determines whether a numeric value represents a missing or invalid observation (NaN).
 * 
 * @param value Floating-point value to test.
 * @return True if value is NaN; false otherwise.
 */
bool is_missing(double value) {
    return std::isnan(value);
}

/**
 * @brief Appends a new numeric column to the dataset.
 * 
 * If the dataset is non-empty, the length of values must match existing row_count().
 * 
 * @param name Unique name for the column.
 * @param values Vector of double-precision numeric values.
 * @throws InvalidArgument If a column with name already exists or value count mismatches row_count().
 */
void Dataset::add_numeric_column(const std::string& name, std::vector<double> values) {
    if (has_column(name)) {
        throw InvalidArgument("Column already exists: " + name);
    }
    if (!col_names_.empty() && values.size() != num_rows_) {
        throw InvalidArgument("Column length mismatch for '" + name + "'");
    }
    if (col_names_.empty()) {
        num_rows_ = values.size();
    }

    col_names_.push_back(name);
    is_numeric_map_[name] = true;
    numeric_cols_[name] = std::move(values);
}

/**
 * @brief Appends a new textual or categorical column to the dataset.
 * 
 * If the dataset is non-empty, the length of values must match existing row_count().
 * 
 * @param name Unique name for the column.
 * @param values Vector of string values.
 * @throws InvalidArgument If a column with name already exists or value count mismatches row_count().
 */
void Dataset::add_text_column(const std::string& name, std::vector<std::string> values) {
    if (has_column(name)) {
        throw InvalidArgument("Column already exists: " + name);
    }
    if (!col_names_.empty() && values.size() != num_rows_) {
        throw InvalidArgument("Column length mismatch for '" + name + "'");
    }
    if (col_names_.empty()) {
        num_rows_ = values.size();
    }

    col_names_.push_back(name);
    is_numeric_map_[name] = false;
    text_cols_[name] = std::move(values);
}

/**
 * @brief Checks if a column with the specified identifier exists in the dataset.
 * 
 * @param name Column name to look up.
 * @return True if found; false otherwise.
 */
bool Dataset::has_column(const std::string& name) const {
    return is_numeric_map_.find(name) != is_numeric_map_.end();
}

/**
 * @brief Checks if a column stores numeric data (double) versus text data (string).
 * 
 * @param name Column name to inspect.
 * @return True if numeric; false if text.
 * @throws DataError If column name is not found.
 */
bool Dataset::is_numeric(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    return is_numeric_map_.at(name);
}

/**
 * @brief Returns the total number of rows in the dataset.
 * 
 * @return Number of rows.
 */
std::size_t Dataset::row_count() const {
    return num_rows_;
}

/**
 * @brief Returns the total number of columns in the dataset.
 * 
 * @return Number of columns.
 */
std::size_t Dataset::column_count() const {
    return col_names_.size();
}

/**
 * @brief Returns the ordered list of all column identifiers in the dataset.
 * 
 * @return Vector of column name strings.
 */
std::vector<std::string> Dataset::column_names() const {
    return col_names_;
}

/**
 * @brief Retrieves a read-only reference to a numeric column's data vector.
 * 
 * @param name Name of the numeric column.
 * @return Const reference to vector of double values.
 * @throws DataError If column is not found or is stored as text.
 */
const std::vector<double>& Dataset::numeric(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    if (!is_numeric(name)) {
        throw DataError("Column '" + name + "' is not numeric");
    }
    return numeric_cols_.at(name);
}

/**
 * @brief Retrieves a read-only reference to a text column's data vector.
 * 
 * @param name Name of the text column.
 * @return Const reference to vector of string values.
 * @throws DataError If column is not found or is stored as numeric.
 */
const std::vector<std::string>& Dataset::text(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    if (is_numeric(name)) {
        throw DataError("Column '" + name + "' is numeric, not text");
    }
    return text_cols_.at(name);
}

/**
 * @brief Creates a new Dataset containing only the rows specified by the index list.
 * 
 * @param indices Vector of 0-based row indices to select.
 * @return Filtered Dataset instance with identical column types and selected rows.
 * @throws InvalidArgument If any index >= row_count().
 */
Dataset Dataset::select_rows(const std::vector<std::size_t>& indices) const {
    for (std::size_t idx : indices) {
        if (idx >= num_rows_) {
            throw InvalidArgument("Row index out of range: " + std::to_string(idx));
        }
    }

    Dataset result;
    for (const auto& col_name : col_names_) {
        if (is_numeric(col_name)) {
            const auto& src_vec = numeric_cols_.at(col_name);
            std::vector<double> new_vec;
            new_vec.reserve(indices.size());
            for (std::size_t idx : indices) {
                new_vec.push_back(src_vec[idx]);
            }
            result.add_numeric_column(col_name, std::move(new_vec));
        } else {
            const auto& src_vec = text_cols_.at(col_name);
            std::vector<std::string> new_vec;
            new_vec.reserve(indices.size());
            for (std::size_t idx : indices) {
                new_vec.push_back(src_vec[idx]);
            }
            result.add_text_column(col_name, std::move(new_vec));
        }
    }
    return result;
}

} // namespace intriviz
