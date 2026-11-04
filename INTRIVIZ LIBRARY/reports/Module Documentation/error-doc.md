# Module Documentation: `error`

**Header File:** `include/cppviz/error.hpp`  
**Implementation File:** `src/cppviz/error.cpp`  
**Subsystem:** Core Utilities & Test Framework  

---

## 1. Overview & Architectural Role

Defines the centralized exception hierarchy for the `cppviz` library, deriving all errors from a common base class `VizError`.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class VizError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidArgument : public VizError {
public:
    using VizError::VizError;
};

class DataError : public VizError {
public:
    using VizError::VizError;
};

class IoError : public VizError {
public:
    using VizError::VizError;
};

void require(bool condition, const std::string& message);

} // namespace cppviz
```

---

## 3. Implementation Details

`require(condition, message)` evaluates `condition` and throws `InvalidArgument(message)` if `false`.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/error.hpp"

void check_val(int val) {
    cppviz::require(val > 0, "Value must be positive");
}
```
