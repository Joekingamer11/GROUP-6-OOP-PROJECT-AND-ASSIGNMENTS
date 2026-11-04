# Module Documentation: `labels`

**Header File:** `include/cppviz/labels.hpp`  
**Implementation File:** `src/cppviz/labels.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

Renders plot titles, X-axis labels, and rotated Y-axis labels outside the active plot frame area.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void draw_title(Axes& ax, const std::string& title);
void draw_xlabel(Axes& ax, const std::string& label);
void draw_ylabel(Axes& ax, const std::string& label);

} // namespace cppviz
```

---

## 3. Implementation Details

- `draw_title`: Horizontally centered in the top margin at `ax.area().y - 25`.
- `draw_xlabel`: Horizontally centered in the bottom margin below tick labels.
- `draw_ylabel`: Rotated 90° counter-clockwise and centered in the left margin.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/labels.hpp"

void add_annotations(cppviz::Axes& ax) {
    cppviz::draw_title(ax, "Sample Title");
    cppviz::draw_xlabel(ax, "Category");
    cppviz::draw_ylabel(ax, "Value ($)");
}
```
