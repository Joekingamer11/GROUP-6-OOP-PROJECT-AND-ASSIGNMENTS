# Module Documentation: `draw`

**Header File:** `include/cppviz/draw.hpp`  
**Implementation File:** `src/cppviz/draw.cpp`  
**Subsystem:** Software Drawing Engine  

---

## 1. Overview & Architectural Role

The `draw` module implements software rasterization primitives: line drawing, filled rectangles, circle outlines, and solid circle fills.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color color, int thickness = 1);
void fill_rect(Canvas& canvas, int x, int y, int w, int h, Color color);
void draw_circle(Canvas& canvas, int cx, int cy, int radius, Color color, int thickness = 1);
void fill_circle(Canvas& canvas, int cx, int cy, int radius, Color color);

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Bresenham's Line Algorithm:** Uses integer error accumulators ($dx, dy, D = 2dy - dx$) to plot 1-pixel lines without floating-point math. Thick lines stamp a filled square brush along the line path.
2. **Circle Rasterization:** Evaluates bounding box pixels $(x - cx)^2 + (y - cy)^2 \le r^2$ for filled circles.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/canvas.hpp"
#include "cppviz/draw.hpp"

int main() {
    cppviz::Canvas canvas(400, 400);
    cppviz::draw_line(canvas, 10, 10, 390, 390, cppviz::Color{0, 0, 255}, 2);
    cppviz::fill_circle(canvas, 200, 200, 50, cppviz::Color{255, 0, 0});
    return 0;
}
```
