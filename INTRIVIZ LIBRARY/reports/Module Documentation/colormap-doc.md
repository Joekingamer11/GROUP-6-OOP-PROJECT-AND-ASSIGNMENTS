# Module Documentation: `colormap`

**Header File:** `include/cppviz/colormap.hpp`  
**Implementation File:** `src/cppviz/colormap.cpp`  
**Subsystem:** Software Drawing Engine  

---

## 1. Overview & Architectural Role

The `colormap` module provides color mathematics, hex parsing (`#RRGGBB`), linear color interpolation (`blend`), continuous colormaps (`viridis`, `magma`, `coolwarm`, `gray`), categorical palettes (`deep`), and ITU-R BT.601 perceived luminance contrast text selection.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

Color color_from_hex(const std::string& hex);
Color blend(Color a, Color b, double t);

double perceived_luminance(Color c);
Color contrast_text_color(Color background);

class Colormap {
public:
    Colormap(const std::vector<Color>& stops);
    Color at(double t) const; // t in [0.0, 1.0]
};

Colormap get_colormap(const std::string& name);
std::vector<Color> categorical_palette(const std::string& name);
Color palette_color(const std::vector<Color>& palette, std::size_t index);

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Perceived Luminance (ITU-R BT.601):**
   $$	ext{Luminance} = 0.299 \cdot R + 0.587 \cdot G + 0.114 \cdot B$$
   If luminance $< 140.0$, `contrast_text_color` returns White (`#FFFFFF`); otherwise Black (`#000000`).
2. **Palette Safety:**
   `palette_color` uses modulo arithmetic (`index % palette.size()`) to prevent array index out-of-bounds crashes when dataset categories exceed palette length.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/colormap.hpp"
#include <iostream>

int main() {
    cppviz::Colormap viridis = cppviz::get_colormap("viridis");
    cppviz::Color mid = viridis.at(0.5);
    std::cout << "Viridis 0.5 RGB: " << (int)mid.r << "," << (int)mid.g << "," << (int)mid.b << "\n";
    return 0;
}
```
