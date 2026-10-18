#include "cppviz/csv_loader.hpp"
#include "cppviz/error.hpp"
#include "cppviz/text_util.hpp"

#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

namespace cppviz {

namespace {

// Helper: checks if a raw string cell matches any configured missing token
bool is_missing_cell(const std::string& cell, const std::vector<std::string>& missing_tokens) {
    for (const auto& token : missing_tokens) {
        if (cell == token) {
            return true;
        }
    }
    return false;
}

// Helper: tokenizes a single line into cells, preserving commas inside double quotes
std::vector<std::string> parse_line(const std::string& line, char delimiter) {
    std::vector<std::string> cells;
    std::string current;
    bool inside_quotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '"') {
            // Handle escaped quotes ("") inside quoted fields or toggle quote state
            if (inside_quotes && i + 1 < line.size() && line[i + 1] == '"') {
                current.push_back('"');
                ++i; // skip second quote character
            } else {
                inside_quotes = !inside_quotes;
            }
        } else if (ch == delimiter && !inside_quotes) {
            cells.push_back(trim(current));
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    cells.push_back(trim(current));
    return cells;
}

} // anonymous namespace

Dataset parse_csv(const std::string& text, const CsvOptions& options) {
    std::vector<std::string> raw_lines;
    std::istringstream stream(text);
    std::string line;

    // 1. Split text into lines, stripping Windows trailing \r and skipping blank lines
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::string trimmed_line = trim(line);
        if (!trimmed_line.empty()) {
            raw_lines.push_back(line);
        }
    }

    if (raw_lines.empty()) {
        return Dataset();
    }

    // 2. Tokenize rows using delimiter and quote tracking
    std::vector<std::vector<std::string>> rows;
    rows.reserve(raw_lines.size());
    for (const auto& l : raw_lines) {
        rows.push_back(parse_line(l, options.delimiter));
    }

    // 3. Extract header or auto-generate column names (col0, col1, ...)
    std::vector<std::string> col_names;
    std::size_t start_row = 0;

    if (options.has_header) {
        if (rows.empty()) {
            return Dataset();
        }
        col_names = rows.front();
        start_row = 1;
    } else {
        if (rows.empty()) {
            return Dataset();
        }
        std::size_t num_cols = rows.front().size();
        for (std::size_t i = 0; i < num_cols; ++i) {
            col_names.push_back("col" + std::to_string(i));
        }
    }

    std::size_t expected_cols = col_names.size();

    // 4. Validate row sizes (ragged-row check) and throw DataError on mismatch
    for (std::size_t r = start_row; r < rows.size(); ++r) {
        if (rows[r].size() != expected_cols) {
            std::size_t line_number = r + 1; // 1-based line index
            throw DataError("DataError: ragged row at line " + std::to_string(line_number) +
                            ": expected " + std::to_string(expected_cols) +
                            " columns, found " + std::to_string(rows[r].size()));
        }
    }

    std::size_t data_row_count = rows.size() - start_row;
    Dataset ds;

    if (expected_cols == 0) {
        return ds;
    }

    // 5. Column-by-column type detection and Dataset construction
    for (std::size_t c = 0; c < expected_cols; ++c) {
        bool is_num = true;

        // Check if all non-missing cells in this column parse cleanly as double numbers
        for (std::size_t r = start_row; r < rows.size(); ++r) {
            const std::string& raw_cell = rows[r][c];

            if (is_missing_cell(raw_cell, options.missing_tokens)) {
                continue; // Skip missing values during type inference
            }

            double val = 0.0;
            if (!try_parse_double(raw_cell, val)) {
                is_num = false;
                break; // Found string token; mark entire column as text
            }
        }

        // Populate typed values into Dataset
        if (is_num) {
            std::vector<double> num_values;
            num_values.reserve(data_row_count);
            for (std::size_t r = start_row; r < rows.size(); ++r) {
                const std::string& raw_cell = rows[r][c];
                if (is_missing_cell(raw_cell, options.missing_tokens)) {
                    num_values.push_back(std::numeric_limits<double>::quiet_NaN());
                } else {
                    double val = 0.0;
                    try_parse_double(raw_cell, val);
                    num_values.push_back(val);
                }
            }
            ds.add_numeric_column(col_names[c], std::move(num_values));
        } else {
            std::vector<std::string> text_values;
            text_values.reserve(data_row_count);
            for (std::size_t r = start_row; r < rows.size(); ++r) {
                const std::string& raw_cell = rows[r][c];
                if (is_missing_cell(raw_cell, options.missing_tokens)) {
                    text_values.push_back("");
                } else {
                    text_values.push_back(raw_cell);
                }
            }
            ds.add_text_column(col_names[c], std::move(text_values));
        }
    }

    return ds;
}

Dataset load_csv(const std::string& path, const CsvOptions& options) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw IoError("cannot open file: " + path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return parse_csv(ss.str(), options);
}

} // namespace cppviz