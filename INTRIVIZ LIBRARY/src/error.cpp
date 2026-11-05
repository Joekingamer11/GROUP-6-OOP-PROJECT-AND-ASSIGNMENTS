/**
 * @file error.cpp
 * @brief Implementation of error assertion helpers and equality operators for basic types.
 */

#include "intriviz/error.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

/**
 * @brief Asserts that a condition is true; throws InvalidArgument if false.
 * 
 * @param condition The boolean expression to test.
 * @param message The error message to include in the thrown InvalidArgument exception.
 * @throws InvalidArgument If condition evaluates to false.
 */
void require(bool condition, const std::string& message) {
    if (!condition) {
        throw InvalidArgument(message);
    }
}

/**
 * @brief Equality comparison operator for Color structures.
 * 
 * @param a First Color to compare.
 * @param b Second Color to compare.
 * @return True if red, green, and blue components are all identical; false otherwise.
 */
bool operator==(const Color& a, const Color& b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

/**
 * @brief Inequality comparison operator for Color structures.
 * 
 * @param a First Color to compare.
 * @param b Second Color to compare.
 * @return True if any RGB channel differs between a and b; false otherwise.
 */
bool operator!=(const Color& a, const Color& b) {
    return !(a == b);
}

} // namespace intriviz
