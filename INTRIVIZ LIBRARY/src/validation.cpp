/**
 * @file validation.cpp
 * @brief Implementation of dataset validation routines and preconditions.
 *
 * This module provides functions to verify prerequisites on Dataset instances
 * such as column presence, correct data types (numeric vs text), non-empty datasets,
 * and matching sequence dimensions.
 */

#include "intriviz/validation.hpp"
#include "intriviz/error.hpp"

namespace intriviz {

/**
 * @brief Asserts that a specified column exists in the dataset.
 * 
 * @param data The dataset to check.
 * @param name The column identifier to search for.
 * @throws DataError If the column does not exist in data.
 */
void require_column(const Dataset& data, const std::string& name) {
    if (!data.has_column(name)) {
        throw DataError("column '" + name + "' not found");
    }
}

/**
 * @brief Asserts that a specified column exists and contains numeric values.
 * 
 * @param data The dataset to check.
 * @param name The numeric column name expected.
 * @throws DataError If the column is missing or is a text column.
 */
void require_numeric_column(const Dataset& data, const std::string& name) {
    require_column(data, name);
    if (!data.is_numeric(name)) {
        throw DataError("column '" + name + "' is not numeric");
    }
}

/**
 * @brief Asserts that a specified column exists and contains textual/categorical values.
 * 
 * @param data The dataset to check.
 * @param name The text column name expected.
 * @throws DataError If the column is missing or is stored as numeric.
 */
void require_text_column(const Dataset& data, const std::string& name) {
    require_column(data, name);
    if (data.is_numeric(name)) {
        throw DataError("column '" + name + "' is numeric, expected text column");
    }
}

/**
 * @brief Asserts that the dataset contains at least one row of data.
 * 
 * @param data The dataset to check.
 * @throws DataError If data.row_count() == 0.
 */
void require_not_empty(const Dataset& data) {
    if (data.row_count() == 0) {
        throw DataError("dataset is empty");
    }
}

/**
 * @brief Asserts that two dimension or size values are identical.
 * 
 * @param a First size value.
 * @param b Second size value.
 * @param what Description of the entities being compared for the error message.
 * @throws InvalidArgument If a != b.
 */
void require_same_size(std::size_t a, std::size_t b, const std::string& what) {
    if (a != b) {
        throw InvalidArgument(what + " size mismatch: " + std::to_string(a) + " != " + std::to_string(b));
    }
}

} // namespace intriviz
