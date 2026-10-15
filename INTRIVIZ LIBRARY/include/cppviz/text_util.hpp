#pragma once
#include <string>
#include <vector>

namespace cppviz {

// Removes leading and trailing spaces, tabs, \r, and \n
std::string trim(const std::string& s);

// Splits string by delimiter, preserving empty fields between delimiters
std::vector<std::string> split(const std::string& s, char delimiter);

// Converts ASCII characters in string to lower-case
std::string to_lower(const std::string& s);

// Attempts to parse a double; returns true only if the entire trimmed string is a valid number
bool try_parse_double(const std::string& s, double& out);

} // namespace cppviz