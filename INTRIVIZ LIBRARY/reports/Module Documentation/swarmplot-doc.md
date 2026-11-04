# Module Documentation: `swarmplot`

**Header File:** `include/cppviz/swarmplot.hpp`  
**Implementation File:** `src/cppviz/swarmplot.cpp`  
**Subsystem:** Statistical Visuals  

---

## 1. Overview & Architectural Role

Renders Seaborn-style categorical swarm plots where individual data points are plotted with iterative horizontal collision displacement to avoid overlap.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct SwarmplotOptions {
    int point_radius = 4;
    bool color_by_category = true;
    Color default_color = Color{76, 114, 176};
};

void swarmplot(Axes& ax, const Dataset& data, const std::string& x_column, const std::string& y_column, const SwarmplotOptions& options = SwarmplotOptions());

} // namespace cppviz
```

---

## 3. Implementation Details & Collision Algorithm

For each category, data points are sorted by Y value. The placement loop tests candidate horizontal offsets ($0, +d, -d, +2d, -2d, \dots$, where $d = 	ext{radius} + 1$). It assigns the first offset where squared distance $(x_1-x_2)^2 + (y_1-y_2)^2 \ge (2r)^2$ is satisfied against all previously placed dots.

---

## 4. Exception Handling

- Throws `DataError` if `x_column` or `y_column` are missing or of incorrect types.
