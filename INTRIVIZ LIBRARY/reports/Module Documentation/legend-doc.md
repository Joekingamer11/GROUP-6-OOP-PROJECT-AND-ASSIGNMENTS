# Module Documentation: `legend`

**Header File:** `include/cppviz/legend.hpp`  
**Implementation File:** `src/cppviz/legend.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

Renders discrete categorical legend boxes with colored swatches.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void draw_legend(Axes& ax, const std::vector<std::string>& labels, const std::vector<Color>& colors, const std::string& title = "");

} // namespace cppviz
```

---

## 3. Implementation Details

Computes legend box dimensions dynamically based on label lengths, rendering a semi-transparent or white background box with a 1-pixel border.
