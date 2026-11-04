# Module Documentation: `preprocess`

**Header File:** `include/cppviz/preprocess.hpp`  
**Implementation File:** `src/cppviz/preprocess.cpp`  
**Subsystem:** Data Management  

---

## 1. Overview & Architectural Role

The `preprocess` module provides data cleaning, filtering, and normalization operations on `Dataset` objects. It is designed to sanitize raw data prior to statistical modeling and plotting.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

Dataset drop_missing(const Dataset& data, const std::vector<std::string>& columns = {});
Dataset fill_missing(const Dataset& data, const std::string& column, double fill_value);
Dataset filter_rows(const Dataset& data, const std::string& numeric_column, double min_val, double max_val);
Dataset normalize(const Dataset& data, const std::string& numeric_column);

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **`drop_missing` (Listwise Deletion):**
   Iterates through specified numeric columns (or all numeric columns if empty). If any designated column contains `std::isnan(value)` for a given row index, that row index is excluded from the returned subset `Dataset`.
2. **`normalize` (Min-Max Scaling):**
   Transforms a numeric column to the range $[0.0, 1.0]$ using:
   $$x' = rac{x - x_{	ext{min}}}{x_{	ext{max}} - x_{	ext{min}}}$$
   If $x_{	ext{max}} == x_{	ext{min}}$, all values are set to $0.0$.

---

## 4. Exception Handling

- Throws `DataError` if specified column names do not exist in the dataset or if non-numeric columns are passed to numeric scaling algorithms.

---

## 5. C++ Usage Example

```cpp
#include "cppviz/dataset.hpp"
#include "cppviz/preprocess.hpp"

int main() {
    cppviz::Dataset data;
    data.add_numeric_column("score", {10.0, std::numeric_limits<double>::quiet_NaN(), 30.0});
    
    cppviz::Dataset clean_data = cppviz::drop_missing(data, {"score"});
    // clean_data now contains 2 rows (10.0 and 30.0)
    return 0;
}
```
