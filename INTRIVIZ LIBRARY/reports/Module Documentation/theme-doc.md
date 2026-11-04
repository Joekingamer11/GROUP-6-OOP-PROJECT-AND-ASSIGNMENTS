# Module Documentation: `theme`

**Header File:** `include/cppviz/theme.hpp`  
**Implementation File:** `src/cppviz/theme.cpp`  
**Subsystem:** Layout & Aesthetics  

---

## 1. Overview & Architectural Role

The `theme` module defines visual styling configurations (background colors, grid line visibility, line thickness, font scales, and axis colors).

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct Theme {
    Color figure_background = Color{255, 255, 255};
    Color plot_background   = Color{255, 255, 255};
    Color grid_color        = Color{230, 230, 230};
    Color axis_color        = Color{30, 30, 30};
    Color text_color        = Color{30, 30, 30};
    bool show_grid          = true;
    int line_thickness      = 1;
    int font_scale          = 1;
};

Theme theme_white();
Theme theme_whitegrid();
Theme theme_darkgrid();
Theme theme_dark();

} // namespace cppviz
```

---

## 3. C++ Usage Example

```cpp
#include "cppviz/theme.hpp"

int main() {
    cppviz::Theme t = cppviz::theme_darkgrid();
    return 0;
}
```
