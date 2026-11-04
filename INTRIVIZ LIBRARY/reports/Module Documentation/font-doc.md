# Module Documentation: `font`

**Header File:** `include/cppviz/font.hpp`  
**Implementation File:** `src/cppviz/font.cpp`  
**Subsystem:** Software Drawing Engine  

---

## 1. Overview & Architectural Role

The `font` module renders ASCII text directly onto a `Canvas` using a built-in 5x7 bitmap font, requiring zero third-party font libraries.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void draw_text(Canvas& canvas, int x, int y, const std::string& text, Color color, int scale = 1);
void draw_text_vertical(Canvas& canvas, int x, int y, const std::string& text, Color color, int scale = 1);

int text_width(const std::string& text, int scale = 1);
int text_height(int scale = 1);

} // namespace cppviz
```

---

## 3. Implementation Details

Each character (ASCII 32-126) is stored as 7 bytes. Bit $j$ of byte $i$ represents pixel state at row $i$, column $j$. `scale` draws $N 	imes N$ pixel blocks per font bit. Vertical text rotates the glyph matrix 90 degrees counter-clockwise (bottom-to-top reading order).

---

## 4. C++ Usage Example

```cpp
#include "cppviz/canvas.hpp"
#include "cppviz/font.hpp"

int main() {
    cppviz::Canvas canvas(200, 100);
    cppviz::draw_text(canvas, 20, 40, "Hello World", cppviz::Color{0, 0, 0}, 2);
    return 0;
}
```
