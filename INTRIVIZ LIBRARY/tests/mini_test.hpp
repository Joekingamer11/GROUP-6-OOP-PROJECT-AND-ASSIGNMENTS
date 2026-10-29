#pragma once
#include <iostream>
#include <cmath>
#include <string>

namespace intriviz {
// Inline global failure count across included test files (C++17)
inline int g_failures = 0;
} // namespace intriviz

#define CHECK(cond) do { \
    if (!(cond)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #cond "\n"; \
        ++::intriviz::g_failures; \
    } \
} while(0)

#define CHECK_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a == _b)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #a " == " #b "\n"; \
        ++::intriviz::g_failures; \
    } \
} while(0)

#define CHECK_NEAR(a, b, tol) do { \
    double _a = (a); \
    double _b = (b); \
    if (std::abs(_a - _b) > (tol)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #a " near " #b \
                  << " (" << _a << " vs " << _b << ", tol " << (tol) << ")\n"; \
        ++::intriviz::g_failures; \
    } \
} while(0)

#define CHECK_THROWS(expr, ExceptionType) do { \
    bool _threw = false; \
    try { \
        expr; \
    } catch (const ExceptionType&) { \
        _threw = true; \
    } catch (...) {} \
    if (!_threw) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #expr " did not throw " #ExceptionType "\n"; \
        ++::intriviz::g_failures; \
    } \
} while(0)

#define CHECK_NOTHROW(expr) do { \
    try { \
        expr; \
    } catch (...) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #expr " threw an exception\n"; \
        ++::intriviz::g_failures; \
    } \
} while(0)

inline int finish() {
    if (::intriviz::g_failures > 0) {
        std::cerr << ::intriviz::g_failures << " test failure(s) total.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}