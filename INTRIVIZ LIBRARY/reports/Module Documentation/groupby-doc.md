# Module Documentation: `groupby`

**Header File:** `include/cppviz/groupby.hpp`  
**Implementation File:** `src/cppviz/groupby.cpp`  
**Subsystem:** Data Management  

---

## 1. Overview & Architectural Role

The `groupby` module aggregates `Dataset` rows according to unique categorical values in a text column. It serves as the data partitioning engine for categorical charts such as `SwarmPlot` and `PointPlot`.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class GroupBy {
public:
    GroupBy(const Dataset& data, const std::string& category_column);

    std::vector<std::string> groups() const;
    std::vector<double> get_group_values(const std::string& group_name, const std::string& value_column) const;
    
    std::map<std::string, double> mean(const std::string& value_column) const;
    std::map<std::string, double> stddev(const std::string& value_column) const;
};

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Categorical Row Index Mapping:**
   Upon construction, `GroupBy` scans the target text column and constructs an internal index map `std::map<std::string, std::vector<std::size_t>> group_indices_`. It preserves the appearance order of categories.
2. **Group-wise Aggregation:**
   Methods like `mean()` and `stddev()` extract numerical slices for each group and compute descriptive statistics natively without full dataset copying.

---

## 4. Exception Handling

- Throws `DataError` if the category column is missing or is a numeric column, or if the requested value column is not numeric.

---

## 5. C++ Usage Example

```cpp
#include "cppviz/groupby.hpp"
#include <iostream>

int main() {
    cppviz::Dataset data;
    data.add_text_column("group", {"A", "B", "A", "B"});
    data.add_numeric_column("val", {10.0, 20.0, 30.0, 40.0});

    cppviz::GroupBy gb(data, "group");
    auto means = gb.mean("val"); // A -> 20.0, B -> 30.0
    std::cout << "Group A Mean: " << means["A"] << "\n";
    return 0;
}
```
