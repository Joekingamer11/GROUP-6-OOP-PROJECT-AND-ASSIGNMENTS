# Module Documentation: `annotation`

**Header File:** `include/cppviz/annotation.hpp`  
**Implementation File:** `src/cppviz/annotation.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

Provides text callouts, threshold reference lines, and directional pointers on top of rendered plots.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void annotate_text(Axes& ax, double x, double y, const std::string& text, Color color = Color{0, 0, 0});
void annotate_hline(Axes& ax, double y_data, Color color, int thickness = 1);
void annotate_vline(Axes& ax, double x_data, Color color, int thickness = 1);
void annotate_arrow(Axes& ax, double x0, double y0, double x1, double y1, Color color = Color{0, 0, 0});

} // namespace cppviz
```

---

## 3. C++ Usage Example

```cpp
#include "cppviz/annotation.hpp"

void add_threshold(cppviz::Axes& ax) {
    cppviz::annotate_hline(ax, 50.0, cppviz::Color{255, 0, 0}, 2); // Red threshold at y=50
}
```
