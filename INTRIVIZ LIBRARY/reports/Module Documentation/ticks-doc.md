# Module Documentation: `ticks`

**Header File:** `include/cppviz/ticks.hpp`  
**Implementation File:** `src/cppviz/ticks.cpp`  
**Subsystem:** Scaling & Mapping  

---

## 1. Overview & Architectural Role

The `ticks` module calculates rounded, human-readable "nice" axis tick intervals (e.g., $0, 5, 10, 15$ instead of $0, 3.71, 7.42$).

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct TickOptions {
    int target_tick_count = 6;
};

std::vector<double> calculate_nice_ticks(double min_val, double max_val, const TickOptions& options = TickOptions());

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Nice Numbers Algorithm:**
   Computes raw step $rac{	ext{range}}{	ext{target\_ticks - 1}}$, determines magnitude $10^{\lfloor \log_{10}(	ext{raw\_step}) floor}$, and rounds the fraction to $1, 2, 5$, or $10$.
2. **Precision Guard:**
   Tick steps are accumulated multiplication-wise ($t_i = 	ext{first\_tick} + i \cdot 	ext{step}$) to avoid binary floating-point accumulation errors ($0.30000000000000004$).

---

## 4. C++ Usage Example

```cpp
#include "cppviz/ticks.hpp"
#include <iostream>

int main() {
    auto ticks = cppviz::calculate_nice_ticks(0.0, 95.0);
    for (double t : ticks) std::cout << t << " "; // 0 20 40 60 80 100
    std::cout << "\n";
    return 0;
}
```
