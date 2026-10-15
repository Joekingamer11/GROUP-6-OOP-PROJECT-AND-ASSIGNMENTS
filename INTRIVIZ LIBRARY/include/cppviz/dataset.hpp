#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace cppviz {

// Returns true if the double value is NaN (missing value)
bool is_missing(double value);

class Dataset {
public:
    Dataset() = default;

    // Add columns (throws InvalidArgument on wrong length or duplicate name)
    void add_numeric_column(const std::string& name, std::vector<double> values);
    void add_text_column(const std::string& name, std::vector<std::string> values);

    // Column queries
    bool has_column(const std::string& name) const;
    bool is_numeric(const std::string& name) const; // Throws DataError if column not found

    // Table dimensions and names
    std::size_t row_count() const;
    std::size_t column_count() const;
    std::vector<std::string> column_names() const; // Preserves insertion order

    // Data accessors (Throw DataError if column missing or wrong type)
    const std::vector<double>& numeric(const std::string& name) const;
    const std::vector<std::string>& text(const std::string& name) const;

    // Subset creation (Throws InvalidArgument if index out of range)
    Dataset select_rows(const std::vector<std::size_t>& indices) const;

private:
    std::vector<std::string> col_names_;
    std::map<std::string, bool> is_numeric_map_;
    std::map<std::string, std::vector<double>> numeric_cols_;
    std::map<std::string, std::vector<std::string>> text_cols_;
    std::size_t num_rows_ = 0;
};

} // namespace cppviz