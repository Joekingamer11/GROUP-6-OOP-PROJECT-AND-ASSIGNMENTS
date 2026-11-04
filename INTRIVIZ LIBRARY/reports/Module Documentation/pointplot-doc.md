# Module Documentation: `pointplot`

**Header File:** `include/cppviz/pointplot.hpp`  
**Implementation File:** `src/cppviz/pointplot.cpp`  
**Subsystem:** Statistical Visuals  

---

## 1. Overview & Architectural Role

Renders categorical point plots showing group means connected by lines, with 95% bootstrap confidence interval error whiskers.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct PointplotOptions {
    bool show_ci = true;
    double confidence_level = 0.95;
    int point_radius = 5;
    Color color = Color{76, 114, 176};
};

void pointplot(Axes& ax, const Dataset& data, const std::string& x_column, const std::string& y_column, const PointplotOptions& options = PointplotOptions());

} // namespace cppviz
```

---

## 3. Implementation Details

Computes group means and non-parametric bootstrap CIs via `stats::bootstrap_ci`. Plots vertical error bar whiskers, category mean dots, and connecting line segments across categories.
