# Module Documentation: `mini_test`

**Header File:** `tests/mini_test.hpp`  
**Subsystem:** Core Utilities & Test Framework  

---

## 1. Overview & Architectural Role

`mini_test.hpp` is a zero-dependency macro-based unit testing harness created to evaluate assertions and integrate seamlessly with CMake and CTest.

---

## 2. API Contract & Macros

```cpp
#define CHECK(cond) ...
#define CHECK_EQ(a, b) ...
#define CHECK_NEAR(a, b, tol) ...
#define CHECK_THROWS(expr, ExceptionType) ...

int finish();
```

---

## 3. Implementation Details

Global failure tracking counts unsatisfied assertions. `CHECK_THROWS` wraps expressions in `try ... catch` blocks to verify exception propagation. `finish()` returns `0` if all checks pass or `1` if failures occurred, signaling test state directly to CTest.

---

## 4. C++ Usage Example

```cpp
#include "mini_test.hpp"

int main() {
    CHECK_EQ(2 + 2, 4);
    CHECK_NEAR(3.14159, 3.14, 0.01);
    return finish();
}
```
