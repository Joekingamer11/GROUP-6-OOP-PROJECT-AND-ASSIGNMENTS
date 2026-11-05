/**
 * @file text_util.cpp
 * @brief Implementation of string manipulation and text parsing utilities.
 *
 * Provides helper functions for whitespace trimming, string splitting by delimiter,
 * case conversion, and robust double-precision floating-point parsing.
 */

#include "intriviz/text_util.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace intriviz {

/**
 * @brief Strips leading and trailing whitespace characters (spaces, tabs, newlines, carriage returns).
 * 
 * @param s The input string to trim.
 * @return A new string with whitespace removed from both ends, or an empty string if s contains only whitespace.
 */
std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

/**
 * @brief Splits a string into substrings separated by a delimiter character.
 * 
 * @param s The string to split.
 * @param delimiter The character used to delineate substrings.
 * @return A vector of token substrings extracted from s.
 */
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::size_t start = 0;
    std::size_t end = s.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

/**
 * @brief Converts all ASCII uppercase characters in a string to lowercase.
 * 
 * @param s The input string.
 * @return A lowercase copy of the input string.
 */
std::string to_lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

/**
 * @brief Attempts to parse a double-precision floating point number from a string.
 * 
 * Trims leading/trailing whitespace before parsing. Succeeds only if the entire
 * trimmed string represents a valid floating-point number.
 * 
 * @param s The string to parse.
 * @param out Output parameter where the parsed double value is stored on success.
 * @return True if parsing succeeded completely; false if the string is empty or contains trailing characters.
 */
bool try_parse_double(const std::string& s, double& out) {
    std::string trimmed = trim(s);
    if (trimmed.empty()) {
        return false;
    }
    char* endptr = nullptr;
    double val = std::strtod(trimmed.c_str(), &endptr);
    if (endptr == trimmed.c_str() || *endptr != '\0') {
        return false;
    }
    out = val;
    return true;
}

} // namespace intriviz
