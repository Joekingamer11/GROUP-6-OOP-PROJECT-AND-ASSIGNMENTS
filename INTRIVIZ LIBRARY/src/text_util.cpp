#include "cppviz/text_util.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace cppviz {

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

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

std::string to_lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

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

} // namespace cppviz