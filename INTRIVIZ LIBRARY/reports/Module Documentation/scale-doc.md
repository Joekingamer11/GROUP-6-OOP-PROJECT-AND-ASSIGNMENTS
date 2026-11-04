# Module Documentation: `scale`

**Header File:** `include/cppviz/scale.hpp`  
**Implementation File:** `src/cppviz/scale.cpp`  
**Subsystem:** Scaling & Mapping  

---

## 1. Overview & Architectural Role

The `scale` module provides abstract and concrete classes for mapping data values to screen pixel coordinates. It establishes the polymorphic base class `Scale` along with continuous linear (`LinearScale`) and logarithmic (`LogScale`) coordinate mapping models.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class Scale {
public:
    virtual ~Scale() = default;

    virtual double map(double data_value) const = 0;
    virtual double invert(double pixel_value) const = 0;
    
    virtual std::vector<double> tick_values() const = 0;
    virtual std::string tick_label(double value) const = 0;
};

class LinearScale : public Scale {
public:
    LinearScale(double data_min, double data_max, double pixel_min, double pixel_max);

    double map(double data_value) const override;
    double invert(double pixel_value) const override;

    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};

class LogScale : public Scale {
public:
    LogScale(double data_min, double data_max, double pixel_min, double pixel_max, double base = 10.0);

    double map(double data_value) const override;
    double invert(double pixel_value) const override;

    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Linear Transformation:**
   $$p = p_{	ext{min}} + rac{x - d_{	ext{min}}}{d_{	ext{max}} - d_{	ext{min}}} \cdot (p_{	ext{max}} - p_{	ext{min}})$$
2. **Logarithmic Transformation:**
   $$p = p_{	ext{min}} + rac{\log_b(x) - \log_b(d_{	ext{min}})}{\log_b(d_{	ext{max}}) - \log_b(d_{	ext{min}})} \cdot (p_{	ext{max}} - p_{	ext{min}})$$
3. **Reversed Coordinate Support:**
   If $p_{	ext{min}} > p_{	ext{max}}$ (as used on the vertical Y-axis where screen pixel $y=0$ is at the top), `map()` automatically scales values upside down so larger data values appear higher on screen.

---

## 4. Exception Handling

- Throws `InvalidArgument` if $d_{	ext{min}} == d_{	ext{max}}$ or if $d_{	ext{min}} \le 0$ for `LogScale`.

---

## 5. C++ Usage Example

```cpp
#include "cppviz/scale.hpp"
#include <iostream>

int main() {
    cppviz::LinearScale scale(0.0, 100.0, 50.0, 450.0);
    std::cout << "Data 50.0 maps to Pixel: " << scale.map(50.0) << "\n"; // 250.0
    return 0;
}
```
