/**
 * @file version.cpp
 * @brief Implementation of library version reporting for Intriviz.
 */

#include "intriviz/Intriviz.hpp"

namespace intriviz {

/**
 * @brief Returns the semantic version string of the Intriviz library.
 * 
 * @return A null-terminated C-style string representing the current library version (e.g., "1.0.0").
 */
const char* version() {
    return "1.0.0";
}

} // namespace intriviz
