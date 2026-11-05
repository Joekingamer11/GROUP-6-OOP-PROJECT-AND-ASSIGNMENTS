/**
 * @file groupby.cpp
 * @brief Implementation of category extraction and split-apply grouping operations.
 *
 * Implements algorithms for identifying unique categorical values while preserving order
 * of appearance, and grouping numeric data values by categorical factors.
 */

#include "intriviz/groupby.hpp"
#include "intriviz/error.hpp"

#include <map>
#include <unordered_set>

namespace intriviz {

/**
 * @brief Returns the unique strings from a collection, preserving first-appearance order.
 * 
 * Uses an unordered set for O(1) duplicate detection while appending unique items to the result vector.
 * 
 * @param items Vector of strings.
 * @return Vector containing each distinct string once, in order of first occurrence.
 */
std::vector<std::string> unique_values(const std::vector<std::string>& items) {
    std::vector<std::string> result;
    std::unordered_set<std::string> seen;
    for (const auto& item : items) {
        if (seen.find(item) == seen.end()) {
            seen.insert(item);
            result.push_back(item);
        }
    }
    return result;
}

/**
 * @brief Groups numeric values by distinct categories defined in a text column.
 * 
 * Validates column types, iterates through the dataset rows, skips missing (NaN) values,
 * and organizes values into Group structures keyed by category name.
 * 
 * @param data Input Dataset.
 * @param category_column Name of the categorical text column defining group keys.
 * @param value_column Name of the numeric column containing observation values.
 * @return Vector of Group objects containing category names and associated value vectors.
 * @throws DataError If either column does not exist or has the incorrect data type.
 */
std::vector<Group> group_by(const Dataset& data,
                            const std::string& category_column,
                            const std::string& value_column) {
    if (!data.has_column(category_column)) {
        throw DataError("Category column '" + category_column + "' not found");
    }
    if (!data.has_column(value_column)) {
        throw DataError("Value column '" + value_column + "' not found");
    }
    if (data.is_numeric(category_column)) {
        throw DataError("Category column '" + category_column + "' must be a text column");
    }
    if (!data.is_numeric(value_column)) {
        throw DataError("Value column '" + value_column + "' must be a numeric column");
    }

    const auto& categories = data.text(category_column);
    const auto& values = data.numeric(value_column);

    std::vector<Group> groups;
    std::map<std::string, std::size_t> key_to_index;

    for (std::size_t i = 0; i < data.row_count(); ++i) {
        double val = values[i];
        if (is_missing(val)) {
            continue; // Skip rows with NaN values
        }

        const std::string& cat = categories[i];
        auto it = key_to_index.find(cat);
        if (it == key_to_index.end()) {
            key_to_index[cat] = groups.size();
            groups.push_back(Group{cat, {val}});
        } else {
            groups[it->second].values.push_back(val);
        }
    }

    return groups;
}

} // namespace intriviz
