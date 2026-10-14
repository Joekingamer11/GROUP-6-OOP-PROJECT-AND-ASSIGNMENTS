#pragma once
#include <cstddef>
#include <string>
#include "cppviz/dataset.hpp"

namespace cppviz {

// Throws DataError if the named column is not present in data.
void require_column(const Dataset& data, const std::string& name);

// Throws DataError if the column does not exist or is not numeric.
void require_numeric_column(const Dataset& data, const std::string& name);

// Throws DataError if the column does not exist or is numeric (holds numbers instead of text).
void require_text_column(const Dataset& data, const std::string& name);

// Throws DataError if dataset has 0 rows.
void require_not_empty(const Dataset& data);

// Throws InvalidArgument if a != b, incorporating 'what' into the error message.
void require_same_size(std::size_t a, std::size_t b, const std::string& what);

} // namespace cppviz