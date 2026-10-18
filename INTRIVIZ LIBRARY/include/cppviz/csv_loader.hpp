#pragma once
#include <string>
#include <vector>
#include "cppviz/dataset.hpp"

namespace cppviz {

struct CsvOptions {
    char delimiter = ',';
    bool has_header = true;      // false: columns are named col0, col1, ...
    std::vector<std::string> missing_tokens = {"", "NA", "N/A", "null", "NULL"};
};

// Reads a file into a string and parses it; throws IoError if the file cannot be opened.
Dataset load_csv(const std::string& path, const CsvOptions& options = {});

// Parses CSV data from an in-memory string.
Dataset parse_csv(const std::string& text, const CsvOptions& options = {});

}