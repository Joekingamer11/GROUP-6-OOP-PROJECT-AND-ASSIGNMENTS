#include "intriviz/preprocess.hpp"
#include "intriviz/error.hpp"
#include "intriviz/stats.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace intriviz {

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