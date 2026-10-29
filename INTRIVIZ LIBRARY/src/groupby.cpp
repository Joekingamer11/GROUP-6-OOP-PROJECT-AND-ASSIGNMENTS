#include "intriviz/groupby.hpp"
#include "intriviz/error.hpp"

#include <map>
#include <unordered_set>

namespace intriviz {

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