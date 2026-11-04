# Module Documentation: `dataset`

**Header File:** `include/cppviz/dataset.hpp`  
**Implementation File:** `src/cppviz/dataset.cpp`  
**Subsystem:** Data Management  

---

## 1. Overview & Architectural Role

The `Dataset` module provides the core tabular data container for the `intriviz` (`cppviz`) library. It acts as an in-memory representation of structured data (similar to a Pandas DataFrame or R data frame). It holds typed columns—specifically double-precision floating-point numbers (`std::vector<double>`) for numerical data and UTF-8 strings (`std::vector<std::string>`) for categorical/text data.

It maintains column insertion order using an internal master column list while offering fast lookup by column name using standard associative maps.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class Dataset {
public:
    Dataset() = default;

    // Column addition
    void add_numeric_column(const std::string& name, const std::vector<double>& values);
    void add_text_column(const std::string& name, const std::vector<std::string>& values);

    // Column queries & accessors
    bool has_column(const std::string& name) const;
    bool is_numeric(const std::string& name) const;
    
    std::size_t row_count() const;
    std::size_t column_count() const;
    std::vector<std::string> column_names() const;

    const std::vector<double>& numeric(const std::string& name) const;
    const std::vector<std::string>& text(const std::string& name) const;

    // Row selection & subsetting
    Dataset select_rows(const std::vector<std::size_t>& indices) const;
};

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Dual Column Maps with Insertion Order:**
   Columns are stored internally in `std::map<std::string, std::vector<double>> numeric_cols_` and `std::map<std::string, std::vector<std::string>> text_cols_`. To preserve insertion order, `std::vector<std::string> col_names_` tracks all added column names.
2. **Missing Value Representation ($NaN$):**
   Missing numerical entries are stored as IEEE 754 Quiet $NaN$ (`std::numeric_limits<double>::quiet_NaN()`), ensuring contiguous vector memory alignment without raw pointer overhead.
3. **Row Count Enforcement:**
   When adding a column to a non-empty dataset, `Dataset` validates that the new vector's length matches `row_count()`. If lengths differ, it throws `InvalidArgument`.

---

## 4. Exception Handling

- Throws `InvalidArgument` if a duplicate column name is added or if a new column's row length mismatches existing columns.
- Throws `DataError` when attempting to access a column that does not exist or querying a numeric accessor on a text column (and vice versa).

---

## 5. C++ Usage Example

```cpp
#include "cppviz/dataset.hpp"
#include <iostream>

int main() {
    cppviz::Dataset ds;
    ds.add_text_column("day", {"Mon", "Tue", "Wed"});
    ds.add_numeric_column("bill", {12.50, 25.00, 18.75});

    std::cout << "Rows: " << ds.row_count() << ", Cols: " << ds.column_count() << "\n";
    for (double val : ds.numeric("bill")) {
        std::cout << "Bill: $" << val << "\n";
    }
    return 0;
}
```
