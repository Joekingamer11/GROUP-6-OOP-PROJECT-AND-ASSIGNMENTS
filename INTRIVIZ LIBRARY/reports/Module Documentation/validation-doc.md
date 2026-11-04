# Module Documentation: `validation`

**Header File:** `include/cppviz/validation.hpp`  
**Implementation File:** `src/cppviz/validation.cpp`  
**Subsystem:** Core Utilities & Test Framework  

---

## 1. Overview & Architectural Role

Provides defensive guard functions that validate dataset structures, column presence, column types, and container dimensions prior to execution.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void require_column(const Dataset& data, const std::string& name);
void require_numeric_column(const Dataset& data, const std::string& name);
void require_text_column(const Dataset& data, const std::string& name);
void require_not_empty(const Dataset& data);
void require_same_size(std::size_t a, std::size_t b, const std::string& what);

} // namespace cppviz
```

---

## 3. Exception Handling

All functions throw specialized `DataError` or `InvalidArgument` exceptions with descriptive error messages detailing missing column names or dimension mismatches.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/validation.hpp"

void process_data(const cppviz::Dataset& ds) {
    cppviz::require_not_empty(ds);
    cppviz::require_numeric_column(ds, "price");
}
```
