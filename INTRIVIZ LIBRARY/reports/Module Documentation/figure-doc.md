# Module Documentation: `figure`

**Header File:** `include/cppviz/figure.hpp`  
**Implementation File:** `src/cppviz/figure.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

Top-level figure container managing canvas allocation, active plot area margins, theme settings, and file serialization.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class Figure {
public:
    Figure(int width = 640, int height = 480);

    Canvas& canvas();
    Axes& axes();

    void set_theme(const Theme& theme);
    void set_margins(const Margins& margins);

    void save(const std::string& filepath) const;

private:
    std::unique_ptr<Canvas> canvas_;
    Axes axes_;
    Theme theme_;
};

} // namespace cppviz
```

---

## 3. Implementation Details & Heap Pointer Ownership

`Figure` owns its `Canvas` via `std::unique_ptr<Canvas>`. This heap allocation ensures that when a `Figure` is moved or returned across function calls, the heap memory location of `Canvas` remains stable, preventing dangling pointer crashes inside `Axes`.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/figure.hpp"

int main() {
    cppviz::Figure fig(800, 600);
    fig.set_theme(cppviz::theme_darkgrid());
    fig.save("fig_output.bmp");
    return 0;
}
```
