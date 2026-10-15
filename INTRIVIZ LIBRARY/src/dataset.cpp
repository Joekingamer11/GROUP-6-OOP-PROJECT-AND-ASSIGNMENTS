#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"

#include <cmath>
#include <utility>

namespace cppviz {

bool is_missing(double value) {
    return std::isnan(value);
}

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

bool Dataset::has_column(const std::string& name) const {
    return is_numeric_map_.find(name) != is_numeric_map_.end();
}

bool Dataset::is_numeric(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    return is_numeric_map_.at(name);
}

std::size_t Dataset::row_count() const {
    return num_rows_;
}

std::size_t Dataset::column_count() const {
    return col_names_.size();
}

std::vector<std::string> Dataset::column_names() const {
    return col_names_;
}

const std::vector<double>& Dataset::numeric(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    if (!is_numeric(name)) {
        throw DataError("Column '" + name + "' is not numeric");
    }
    return numeric_cols_.at(name);
}

const std::vector<std::string>& Dataset::text(const std::string& name) const {
    if (!has_column(name)) {
        throw DataError("Column '" + name + "' not found");
    }
    if (is_numeric(name)) {
        throw DataError("Column '" + name + "' is numeric, not text");
    }
    return text_cols_.at(name);
}

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

} // namespace cppviz
