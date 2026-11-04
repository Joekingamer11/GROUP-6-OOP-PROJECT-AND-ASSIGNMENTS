# Module Documentation: `canvas`

**Header File:** `include/cppviz/canvas.hpp`  
**Implementation File:** `src/cppviz/canvas.cpp`  
**Subsystem:** Software Drawing Engine  

---

## 1. Overview & Architectural Role

`Canvas` represents an uncompressed 2D RGB image memory buffer. It is the destination target for all software rasterization and rendering routines in `cppviz`.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

class Canvas {
public:
    Canvas(int width, int height, Color background = Color{255, 255, 255});

    int width() const;
    int height() const;
    
    void set_pixel(int x, int y, Color color);
    Color get_pixel(int x, int y) const;
    
    void clear(Color color);
    const std::vector<std::uint8_t>& buffer() const; // Row-major RGB bytes (width * height * 3)
};

} // namespace cppviz
```

---

## 3. Implementation Details

Stores pixels in a contiguous `std::vector<std::uint8_t>` of size $	ext{width} 	imes 	ext{height} 	imes 3$. Pixel $(x,y)$ starts at index $(y 	imes 	ext{width} + x) 	imes 3$. Out-of-bounds `set_pixel` calls are clipped silently, while out-of-bounds `get_pixel` calls throw `InvalidArgument`.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/canvas.hpp"

int main() {
    cppviz::Canvas canvas(640, 480, cppviz::Color{255, 255, 255});
    canvas.set_pixel(100, 100, cppviz::Color{255, 0, 0}); // Red pixel
    return 0;
}
```
