# Module Documentation: `types`

**Header File:** `include/cppviz/types.hpp`  
**Implementation File:** `src/cppviz/types.cpp`  
**Subsystem:** Core Utilities & Test Framework  

---

## 1. Overview & Architectural Role

Defines core POD (Plain Old Data) structs for colors, pixel positions, data coordinates, rectangular boundaries, and plot margins.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
};

struct PointI {
    int x = 0;
    int y = 0;
};

struct PointD {
    double x = 0.0;
    double y = 0.0;
};

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

struct Margins {
    int left = 70;
    int right = 20;
    int top = 40;
    int bottom = 60;
};

bool operator==(const Color& a, const Color& b);
bool operator!=(const Color& a, const Color& b);

} // namespace cppviz
```

---

## 3. C++ Usage Example

```cpp
#include "cppviz/types.hpp"

int main() {
    cppviz::Color c1{255, 0, 0};
    cppviz::Color c2{255, 0, 0};
    bool is_same = (c1 == c2); // true
    return 0;
}
```
