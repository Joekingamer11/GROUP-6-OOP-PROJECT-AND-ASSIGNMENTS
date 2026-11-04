# Module Documentation: `image_export`

**Header File:** `include/cppviz/image_export.hpp`  
**Implementation File:** `src/cppviz/image_export.cpp`  
**Subsystem:** Software Drawing Engine  

---

## 1. Overview & Architectural Role

The `image_export` module serializes `Canvas` pixel buffers into uncompressed 24-bit Bitmap (`.bmp`) and Portable Pixmap (`.ppm`) binary files.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

void save_bmp(const Canvas& canvas, const std::string& filepath);
void save_ppm(const Canvas& canvas, const std::string& filepath);
void save_image(const Canvas& canvas, const std::string& filepath); // Routes by extension

} // namespace cppviz
```

---

## 3. Implementation Details & Key Serialization Rules

1. **24-bit BMP File Format:** Writes 14-byte `BITMAPFILEHEADER` (signature `BM`, size, offset 54) and 40-byte `BITMAPINFOHEADER`.
2. **Row Padding & Byte Order:** BMP pixels are stored bottom-row-first in Blue-Green-Red (BGR) order. Each scanline is padded with $0..3$ zero bytes to align to 4-byte boundaries.

---

## 4. C++ Usage Example

```cpp
#include "cppviz/canvas.hpp"
#include "cppviz/image_export.hpp"

int main() {
    cppviz::Canvas canvas(200, 200, cppviz::Color{255, 255, 255});
    cppviz::save_image(canvas, "output.bmp");
    return 0;
}
```
