# Module Documentation: `text_util`

**Header File:** `include/cppviz/text_util.hpp`  
**Implementation File:** `src/cppviz/text_util.cpp`  
**Subsystem:** Core Utilities & Test Framework  

---

## 1. Overview & Architectural Role

Provides string manipulation utilities for whitespace trimming, splitting, case conversion, and fixed-precision floating-point formatting.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string to_lower(const std::string& s);
std::string format_float(double val, int decimals = 2);

} // namespace cppviz
```

---

## 3. Implementation Details

`format_float` formats numbers via `std::ostringstream` with `std::fixed` and `std::setprecision(decimals)` to ensure clean numerical string formatting without trailing floating-point noise.
