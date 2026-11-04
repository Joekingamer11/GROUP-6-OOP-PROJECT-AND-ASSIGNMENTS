# Module Documentation: `csv_loader`

**Header File:** `include/cppviz/csv_loader.hpp`  
**Implementation File:** `src/cppviz/csv_loader.cpp`  
**Subsystem:** Data Management  

---

## 1. Overview & Architectural Role

The `csv_loader` module is responsible for parsing Comma-Separated Values (CSV) text files into structured `Dataset` instances. It features a custom finite-state machine (FSM) parser designed to handle escaped fields, quotes, custom missing value tokens, and automatic numeric type inference without external dependencies.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct CsvOptions {
    char delimiter = ',';
    bool has_header = true;
    std::vector<std::string> missing_tokens = {"", "NA", "NaN", "null", "NULL"};
};

Dataset parse_csv(std::istream& input, const CsvOptions& options = CsvOptions());
Dataset load_csv(const std::string& filepath, const CsvOptions& options = CsvOptions());

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Deterministic State-Machine Tokenizer:**
   The parser reads characters sequentially, maintaining a boolean state `in_quotes`. Commas (`,`) inside quoted strings (e.g., `"New York, NY"`) are preserved as literal characters rather than field delimiters. Two consecutive double quotes (`""`) within a quoted string are unescaped to a single `"`.
2. **Automatic Type Inference:**
   After tokenizing columns into raw string vectors, `parse_csv` tests every non-missing cell in a column using `std::strtod`. If every valid cell parses into a double float, the column is instantiated as a numeric column (`std::vector<double>`), converting missing tokens to $NaN$. Otherwise, it is instantiated as a text column.

---

## 4. Exception Handling

- Throws `IoError` if `load_csv()` fails to open the specified file path.
- Throws `DataError` if the CSV file is completely empty or contains inconsistent column counts across rows.

---

## 5. C++ Usage Example

```cpp
#include "cppviz/csv_loader.hpp"
#include <iostream>

int main() {
    try {
        cppviz::Dataset ds = cppviz::load_csv("data/input/tips_sample.csv");
        std::cout << "Successfully loaded CSV with " << ds.row_count() << " rows.\n";
    } catch (const cppviz::IoError& e) {
        std::cerr << "File Error: " << e.what() << "\n";
    }
    return 0;
}
```
