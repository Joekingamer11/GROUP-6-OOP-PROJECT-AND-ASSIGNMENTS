# Module Documentation: `categorical_scale`

**Header File:** `include/cppviz/categorical_scale.hpp`  
**Implementation File:** `src/cppviz/categorical_scale.cpp`  
**Subsystem:** Scaling & Mapping  

---

## 1. Overview & Architectural Role

`CategoricalScale` maps discrete text category labels to central pixel positions and bandwidths along an axis.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class CategoricalScale : public Scale {
public:
    CategoricalScale(const std::vector<std::string>& categories, double pixel_min, double pixel_max);

    double map(double index) const override;
    double map_category(const std::string& category) const;
    double band_width() const;

    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};

} // namespace cppviz
```

---

## 3. Implementation Details

Divides screen range $p_{	ext{max}} - p_{	ext{min}}$ into $N$ equal bands of width $W = rac{	ext{range}}{N}$. Category $i$ maps to center pixel position $p_{	ext{min}} + (i + 0.5) \cdot W$.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/categorical_scale.hpp"
#include <iostream>

int main() {
    cppviz::CategoricalScale cs({"Thu", "Fri", "Sat", "Sun"}, 0.0, 400.0);
    std::cout << "Fri center pixel: " << cs.map_category("Fri") << "\n"; // 150.0
    return 0;
}
```
