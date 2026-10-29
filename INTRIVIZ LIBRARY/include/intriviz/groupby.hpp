#pragma once

#include <string>
#include <vector>
#include "intriviz/dataset.hpp"

namespace intriviz {

struct Group {
    std::string key;
    std::vector<double> values;
};

// Returns unique string values in first-appearance order, with no repeats
std::vector<std::string> unique_values(const std::vector<std::string>& items);

// category_column must be text, value_column numeric. Rows with missing values (NaN) are skipped.
std::vector<Group> group_by(const Dataset& data,
                            const std::string& category_column,
                            const std::string& value_column);

} // namespace intriviz