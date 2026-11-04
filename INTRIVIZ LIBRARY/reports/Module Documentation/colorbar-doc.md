# Module Documentation: `colorbar`

**Header File:** `include/cppviz/colorbar.hpp`  
**Implementation File:** `src/cppviz/colorbar.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

Renders vertical color legend gradient bars next to continuous heatmaps.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void draw_colorbar(Axes& ax, const Colormap& colormap, double vmin, double vmax, const std::string& label = "");

} // namespace cppviz
```

---

## 3. Implementation Details

Positions a 15px-wide gradient bar 15 pixels to the right of `ax.area()`. Iterates vertically from $y_{	ext{bottom}}$ ($t=0.0$) to $y_{	ext{top}}$ ($t=1.0$), drawing 1-pixel horizontal gradient lines sampled from `colormap.at(t)`. Renders min, midpoint, and max numerical tick labels.

---

## 4. Exception Handling

- Throws `InvalidArgument` if $v_{	ext{min}} \ge v_{	ext{max}}$.
