#include "mini_test.hpp"
#include "cppviz/image_export.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/error.hpp"
#include "cppviz/types.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace {

// Helper to read an entire binary file into a byte buffer
std::vector<std::uint8_t> read_binary_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in.is_open()) {
        return {};
    }
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> buffer(static_cast<std::size_t>(size));
    if (size > 0) {
        in.read(reinterpret_cast<char*>(buffer.data()), size);
    }
    return buffer;
}

// Little-endian multi-byte readers
std::uint16_t read_u16(const std::vector<std::uint8_t>& b, std::size_t offset) {
    return static_cast<std::uint16_t>(b[offset] | (b[offset + 1] << 8));
}

std::uint32_t read_u32(const std::vector<std::uint8_t>& b, std::size_t offset) {
    return static_cast<std::uint32_t>(
        b[offset] |
        (b[offset + 1] << 8) |
        (b[offset + 2] << 16) |
        (b[offset + 3] << 24)
    );
}

} // anonymous namespace

int main() {
    using namespace cppviz;

    const std::string ppm_test_file = "temp_test_image.ppm";
    const std::string bmp_test_file = "temp_test_image.bmp";
    const std::string save_ppm_file = "temp_test_save.ppm";
    const std::string save_bmp_file = "temp_test_save.bmp";

    // Create a 3x2 Canvas fixture:
    // Row 0 (Top):    (0,0)=Red      (1,0)=Green    (2,0)=Blue
    // Row 1 (Bottom): (0,1)=Yellow   (1,1)=White    (2,1)=Black
    Canvas canvas(3, 2, Color{0, 0, 0});
    canvas.set_pixel(0, 0, Color{255, 0, 0});    // Red
    canvas.set_pixel(1, 0, Color{0, 255, 0});    // Green
    canvas.set_pixel(2, 0, Color{0, 0, 255});    // Blue
    canvas.set_pixel(0, 1, Color{255, 255, 0});  // Yellow (R=255, G=255, B=0)
    canvas.set_pixel(1, 1, Color{255, 255, 255}); // White
    canvas.set_pixel(2, 1, Color{0, 0, 0});      // Black

    // ========================================================================
    // 1. Netpbm PPM (P6) Export Tests
    // ========================================================================
    write_ppm(canvas, ppm_test_file);
    auto ppm_bytes = read_binary_file(ppm_test_file);
    CHECK(ppm_bytes.size() >= 13);

    // ASCII header check
    std::string header_str(ppm_bytes.begin(), ppm_bytes.begin() + 11);
    CHECK_EQ(header_str, std::string("P6\n3 2\n255\n"));

    // Total file size: 11 header bytes + 18 pixel bytes = 29 bytes
    CHECK_EQ(ppm_bytes.size(), static_cast<std::size_t>(29));

    // First pixel (0,0) in top row: Red (255, 0, 0)
    CHECK_EQ(ppm_bytes[11], 255);
    CHECK_EQ(ppm_bytes[12], 0);
    CHECK_EQ(ppm_bytes[13], 0);

    // ========================================================================
    // 2. Windows 24-bit BMP Export Tests
    // ========================================================================
    write_bmp(canvas, bmp_test_file);
    auto bmp_bytes = read_binary_file(bmp_test_file);

    // 54 header bytes + (3 * 3 bytes + 3 padding bytes) * 2 rows = 78 bytes total
    CHECK_EQ(bmp_bytes.size(), static_cast<std::size_t>(78));

    // BITMAPFILEHEADER verification
    CHECK_EQ(bmp_bytes[0], static_cast<std::uint8_t>('B'));
    CHECK_EQ(bmp_bytes[1], static_cast<std::uint8_t>('M'));
    CHECK_EQ(read_u32(bmp_bytes, 2), static_cast<std::uint32_t>(78)); // bfSize
    CHECK_EQ(read_u16(bmp_bytes, 6), static_cast<std::uint16_t>(0));  // bfReserved1
    CHECK_EQ(read_u16(bmp_bytes, 8), static_cast<std::uint16_t>(0));  // bfReserved2
    CHECK_EQ(read_u32(bmp_bytes, 10), static_cast<std::uint32_t>(54));// bfOffBits

    // BITMAPINFOHEADER verification
    CHECK_EQ(read_u32(bmp_bytes, 14), static_cast<std::uint32_t>(40)); // biSize
    CHECK_EQ(read_u32(bmp_bytes, 18), static_cast<std::uint32_t>(3));  // biWidth
    CHECK_EQ(read_u32(bmp_bytes, 22), static_cast<std::uint32_t>(2));  // biHeight
    CHECK_EQ(read_u16(bmp_bytes, 26), static_cast<std::uint16_t>(1));  // biPlanes
    CHECK_EQ(read_u16(bmp_bytes, 28), static_cast<std::uint16_t>(24)); // biBitCount
    CHECK_EQ(read_u32(bmp_bytes, 30), static_cast<std::uint32_t>(0));  // biCompression
    CHECK_EQ(read_u32(bmp_bytes, 34), static_cast<std::uint32_t>(24)); // biSizeImage

    // Scanline 0 (canvas bottom row y = 1):
    // Pixel (0, 1) is Yellow: R=255, G=255, B=0 -> BMP stores BGR: (0, 255, 255)
    CHECK_EQ(bmp_bytes[54 + 0], 0);   // B
    CHECK_EQ(bmp_bytes[54 + 1], 255); // G
    CHECK_EQ(bmp_bytes[54 + 2], 255); // R

    // Pixel (1, 1) is White: (255, 255, 255)
    CHECK_EQ(bmp_bytes[54 + 3], 255);
    CHECK_EQ(bmp_bytes[54 + 4], 255);
    CHECK_EQ(bmp_bytes[54 + 5], 255);

    // Pixel (2, 1) is Black: (0, 0, 0)
    CHECK_EQ(bmp_bytes[54 + 6], 0);
    CHECK_EQ(bmp_bytes[54 + 7], 0);
    CHECK_EQ(bmp_bytes[54 + 8], 0);

    // Scanline 0 row padding (3 zero bytes)
    CHECK_EQ(bmp_bytes[54 + 9], 0);
    CHECK_EQ(bmp_bytes[54 + 10], 0);
    CHECK_EQ(bmp_bytes[54 + 11], 0);

    // Scanline 1 (canvas top row y = 0, offset 54 + 12 = 66):
    // Pixel (0, 0) is Red: R=255, G=0, B=0 -> BMP stores BGR: (0, 0, 255)
    CHECK_EQ(bmp_bytes[66 + 0], 0);   // B
    CHECK_EQ(bmp_bytes[66 + 1], 0);   // G
    CHECK_EQ(bmp_bytes[66 + 2], 255); // R

    // Pixel (1, 0) is Green: (0, 255, 0)
    CHECK_EQ(bmp_bytes[66 + 3], 0);
    CHECK_EQ(bmp_bytes[66 + 4], 255);
    CHECK_EQ(bmp_bytes[66 + 5], 0);

    // Pixel (2, 0) is Blue: (255, 0, 0 in BGR)
    CHECK_EQ(bmp_bytes[66 + 6], 255);
    CHECK_EQ(bmp_bytes[66 + 7], 0);
    CHECK_EQ(bmp_bytes[66 + 8], 0);

    // Scanline 1 row padding
    CHECK_EQ(bmp_bytes[66 + 9], 0);
    CHECK_EQ(bmp_bytes[66 + 10], 0);
    CHECK_EQ(bmp_bytes[66 + 11], 0);

    // ========================================================================
    // 3. save_image Routing & Case Sensitivity
    // ========================================================================
    save_image(canvas, save_bmp_file);
    CHECK_EQ(read_binary_file(save_bmp_file).size(), static_cast<std::size_t>(78));

    save_image(canvas, save_ppm_file);
    CHECK_EQ(read_binary_file(save_ppm_file).size(), static_cast<std::size_t>(29));

    save_image(canvas, "temp_test_upper.BMP");
    CHECK_EQ(read_binary_file("temp_test_upper.BMP").size(), static_cast<std::size_t>(78));
    std::remove("temp_test_upper.BMP");

    // ========================================================================
    // 4. Exception & Error Handling
    // ========================================================================
    CHECK_THROWS(save_image(canvas, "invalid_format.xyz"), InvalidArgument);
    CHECK_THROWS(save_image(canvas, "no_extension_file"), InvalidArgument);

    CHECK_THROWS(write_ppm(canvas, "/non_existent_folder_xyz/img.ppm"), IoError);
    CHECK_THROWS(write_bmp(canvas, "/non_existent_folder_xyz/img.bmp"), IoError);

    // ========================================================================
    // 5. Cleanup Temporary Artifacts
    // ========================================================================
    std::remove(ppm_test_file.c_str());
    std::remove(bmp_test_file.c_str());
    std::remove(save_ppm_file.c_str());
    std::remove(save_bmp_file.c_str());

    return finish();
}