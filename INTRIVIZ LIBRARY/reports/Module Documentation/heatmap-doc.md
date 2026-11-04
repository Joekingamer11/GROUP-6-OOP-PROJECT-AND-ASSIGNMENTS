# Module Documentation: `heatmap`

**Header File:** `include/cppviz/heatmap.hpp`  
**Implementation File:** `src/cppviz/heatmap.cpp`  
**Subsystem:** Statistical Visuals  

---

## 1. Overview & Architectural Role

Renders 2D correlation matrix heatmaps with colormap cell shading, optional numerical text annotations, and automatic luminance-based text contrast.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct HeatmapOptions {
    std::string colormap = "viridis";
    bool annotate = true;
    int decimals = 2;
    bool show_colorbar = true;
};

void heatmap(Axes& ax, const std::vector<std::vector<double>>& matrix, 
             const std::vector<std::string>& row_labels, 
             const std::vector<std::string>& col_labels, 
             const HeatmapOptions& options = HeatmapOptions());

} // namespace cppviz
```

---

## 3. Implementation Details

Evaluates cell colors by normalizing values $t = rac{v - v_{	ext{min}}}{v_{	ext{max}} - v_{	ext{min}}}$ and sampling `Colormap::at(t)`. Uses ITU-R BT.601 perceived luminance to choose black or white annotation text.
