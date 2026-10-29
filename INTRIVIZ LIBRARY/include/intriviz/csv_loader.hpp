#pragma once
#include <string>
#include <vector>
#include "intriviz/dataset.hpp"

namespace intriviz {
/// Options for configuring CSV file parsing.
struct CsvOptions {
    char delimiter = ',';         ///< Character separating cells
    bool has_header = true;      ///< True if first row contains column names
    std::vector<std::string> missing_tokens = {"", "NA", "N/A", "null", "NULL"}; ///< Tokens treated as NaN/missing values
};
/// Reads a CSV file from disk into a Dataset object.
/// @param path Absolute or relative file path.
/// @param options CSV formatting and delimiter options.
/// @return Populated Dataset instance.
/// @throws IoError If file cannot be opened.
/// @throws DataError If ragged rows are detected.

// Reads a file into a string and parses it; throws IoError if the file cannot be opened.
Dataset load_csv(const std::string& path, const CsvOptions& options = {});

/// Parses CSV data directly from an in-memory string.
/// @param text CSV string content.
/// @param options CSV formatting options.
/// @return Populated Dataset instance.
/// @throws DataError If cell counts vary across rows.

// Parses CSV data from an in-memory string.
Dataset parse_csv(const std::string& text, const CsvOptions& options = {});

}