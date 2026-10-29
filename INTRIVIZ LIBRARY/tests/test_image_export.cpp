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

// Reads a binary file into a byte buffer
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

// Big-endian 32-bit reader for PNG chunk verification
std::uint32_t read_u32_be(const std::vector<std::uint8_t>& b, std::size_t offset) {
    return static_cast<std::uint32_t>(
        (b[offset] << 24) |
        (b[offset + 1] << 16) |
        (b[offset + 2] << 8) |
        b[offset + 3]
    );
}

} // anonymous namespace

int main() {
    using namespace cppviz;

    const std::string ppm_file = "temp_test_image.ppm";
    const std::string bmp_file = "temp_test_image.bmp";
    const std::string png_file = "temp_test_image.png";

    // 2x2 Canvas fixture
    Canvas canvas(2, 2, Color{255, 255, 255});
    canvas.set_pixel(0, 0, Color{255, 0, 0});   // Red
    canvas.set_pixel(1, 0, Color{0, 255, 0});   // Green
    canvas.set_pixel(0, 1, Color{0, 0, 255});   // Blue
    canvas.set_pixel(1, 1, Color{0, 0, 0});     // Black

    // --- 1. PPM Export Tests ---
    write_ppm(canvas, ppm_file);
    auto ppm_bytes = read_binary_file(ppm_file);
    CHECK(ppm_bytes.size() > 0);
    std::remove(ppm_file.c_str());

    // --- 2. BMP Export Tests ---
    write_bmp(canvas, bmp_file);
    auto bmp_bytes = read_binary_file(bmp_file);
    CHECK_EQ(bmp_bytes[0], 'B'); // FIXED: Added [0]
    CHECK_EQ(bmp_bytes[1], 'M'); // FIXED: Changed index from 7 to 1
    std::remove(bmp_file.c_str());

    // --- 3. PNG Export & Signature Tests ---
    write_png(canvas, png_file);
    auto png_bytes = read_binary_file(png_file);
    CHECK(png_bytes.size() >= 57);

    // PNG 8-byte magic header
    // FIXED: Corrected all indices to start at 0
    CHECK_EQ(png_bytes[0], 0x89);
    CHECK_EQ(png_bytes[1], 0x50); // 'P'
    CHECK_EQ(png_bytes[2], 0x4E); // 'N'
    CHECK_EQ(png_bytes[3], 0x47); // 'G'
    CHECK_EQ(png_bytes[4], 0x0D);
    CHECK_EQ(png_bytes[5], 0x0A);
    CHECK_EQ(png_bytes[6], 0x1A);
    CHECK_EQ(png_bytes[7], 0x0A);

    // Verify IHDR Chunk
    CHECK_EQ(read_u32_be(png_bytes, 8), 13); // IHDR length
    CHECK_EQ(png_bytes[12], 'I'); // FIXED: Index 12
    CHECK_EQ(png_bytes[13], 'H'); // FIXED: Index 13
    CHECK_EQ(png_bytes[14], 'D'); // FIXED: Index 14
    CHECK_EQ(png_bytes[15], 'R'); // FIXED: Index 15
    CHECK_EQ(read_u32_be(png_bytes, 16), 2); // width = 2
    CHECK_EQ(read_u32_be(png_bytes, 20), 2); // height = 2
    CHECK_EQ(png_bytes[24], 8);              // FIXED: bit depth at index 24
    CHECK_EQ(png_bytes[25], 2);              // FIXED: color type at index 25

    // --- 4. save_image() Dispatcher & Extensions ---
    save_image(canvas, "temp_test_upper.PNG");
    CHECK(read_binary_file("temp_test_upper.PNG").size() >= 57);
    std::remove("temp_test_upper.PNG");

    save_image(canvas, "temp_test_lower.png");
    CHECK(read_binary_file("temp_test_lower.png").size() >= 57);
    std::remove("temp_test_lower.png");

    CHECK_THROWS(save_image(canvas, "invalid.tiff"), InvalidArgument);
    CHECK_THROWS(save_image(canvas, "no_extension"), InvalidArgument);
    CHECK_THROWS(write_png(canvas, "/invalid_directory_xyz/out.png"), IoError);

    std::remove(png_file.c_str());

    return finish();
}