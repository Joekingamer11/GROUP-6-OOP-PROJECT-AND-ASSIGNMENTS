# Module Documentation: `quick`

**Header File:** `include/cppviz/quick.hpp`  
**Implementation File:** `src/cppviz/quick.cpp`  
**Subsystem:** Statistical Visuals  

---

## 1. Overview & Architectural Role

The `quick` module provides a high-level, 3-line convenience API for instant plot generation (similar to Seaborn's `sns.swarmplot`, `sns.pointplot`, `sns.heatmap`).

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

Figure quick_swarmplot(const Dataset& data, const std::string& x_column, const std::string& y_column, const std::string& title = "");
Figure quick_pointplot(const Dataset& data, const std::string& x_column, const std::string& y_column, const std::string& title = "");
Figure quick_heatmap(const Dataset& data, const std::vector<std::string>& numeric_columns, const std::string& title = "");

} // namespace cppviz
```

---

## 3. C++ Usage Example

```cpp
#include "cppviz/quick.hpp"
#include "cppviz/csv_loader.hpp"

int main() {
    auto ds = cppviz::load_csv("data/input/tips_sample.csv");
    auto fig = cppviz::quick_swarmplot(ds, "day", "total_bill", "Swarm Plot Example");
    fig.save("quick_swarm.bmp");
    return 0;
}
```
