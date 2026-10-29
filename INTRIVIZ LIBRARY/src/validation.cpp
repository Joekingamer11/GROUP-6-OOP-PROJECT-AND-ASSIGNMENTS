#include "intriviz/validation.hpp"
#include "intriviz/error.hpp"

namespace intriviz {

void require_column(const Dataset& data, const std::string& name) {
    if (!data.has_column(name)) {
        throw DataError("column '" + name + "' not found");
    }
}

void require_numeric_column(const Dataset& data, const std::string& name) {
    require_column(data, name);
    if (!data.is_numeric(name)) {
        throw DataError("column '" + name + "' is not numeric");
    }
}

void require_text_column(const Dataset& data, const std::string& name) {
    require_column(data, name);
    if (data.is_numeric(name)) {
        throw DataError("column '" + name + "' is numeric, expected text column");
    }
}

void require_not_empty(const Dataset& data) {
    if (data.row_count() == 0) {
        throw DataError("dataset is empty");
    }
}

void require_same_size(std::size_t a, std::size_t b, const std::string& what) {
    if (a != b) {
        throw InvalidArgument(what + " size mismatch: " + std::to_string(a) + " != " + std::to_string(b));
    }
}

} // namespace intriviz