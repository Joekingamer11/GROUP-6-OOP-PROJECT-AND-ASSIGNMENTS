# Module Documentation: `axes`

**Header File:** `include/cppviz/axes.hpp`  
**Implementation File:** `src/cppviz/axes.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

`Axes` manages plot coordinate frames, converting continuous data space to canvas pixel coordinates via `Scale` objects, and rendering frame borders, grid lines, ticks, and tick labels.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class Axes {
public:
    Axes(Canvas& canvas, Rect area, const Theme& theme);

    const Rect& area() const;
    void set_area(const Rect& area);

    Canvas& canvas();
    const Theme& theme() const;

    std::pair<double, double> x_pixel_range() const;
    std::pair<double, double> y_pixel_range() const;

    void set_x_scale(std::shared_ptr<Scale> scale);
    void set_y_scale(std::shared_ptr<Scale> scale);

    const Scale& x_scale() const;
    const Scale& y_scale() const;

    PointI to_pixel(double x, double y) const;
    void draw_frame();
};

} // namespace cppviz
```

---

## 3. Implementation Details & Memory Safety

1. **Reversed Y-Axis Range:** `y_pixel_range()` returns `{area_.y + area_.h, area_.y}` so smaller Y pixel values correspond to higher data points.
2. **Stable Pointer Access (`Canvas* canvas_`):** `Axes` holds a pointer to `Canvas` rather than a reference, ensuring object validity when a `Figure` is moved across stack frames.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/axes.hpp"

int main() {
    cppviz::Canvas canvas(640, 480);
    cppviz::Rect area{70, 40, 550, 380};
    cppviz::Axes ax(canvas, area, cppviz::theme_whitegrid());
    return 0;
}
```
