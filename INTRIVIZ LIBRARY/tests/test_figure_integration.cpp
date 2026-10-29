#include "mini_test.hpp"
#include "intriviz/figure.hpp"
#include "intriviz/axes.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/font.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {

// Helper function to read binary file contents into a byte buffer
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

// Helper to read 32-bit unsigned integer from little-endian byte array
std::uint32_t read_u32_le(const std::vector<std::uint8_t>& b, std::size_t offset) {
    if (offset + 4 > b.size()) return 0;
    return static_cast<std::uint32_t>(
        b[offset] |
        (b[offset + 1] << 8) |
        (b[offset + 2] << 16) |
        (b[offset + 3] << 24)
    );
}

// Helper to read 16-bit unsigned integer from little-endian byte array
std::uint16_t read_u16_le(const std::vector<std::uint8_t>& b, std::size_t offset) {
    if (offset + 2 > b.size()) return 0;
    return static_cast<std::uint16_t>(b[offset] | (b[offset + 1] << 8));
}

} // anonymous namespace

int main() {
    using namespace intriviz;

    const std::string test_output_path = "temp_figure_integration_test.bmp";

    std::filesystem::create_directories("data/output");

    // 1. Create a Figure with custom dimensions and theme
    {
        Figure fig(640, 480, theme_whitegrid());
        
        // Verify default canvas dimensions
        CHECK_EQ(fig.canvas().width(), 640);
        CHECK_EQ(fig.canvas().height(), 480);

        // Verify axes area positioning inside figure
        const Rect& area = fig.axes().area();
        CHECK(area.w > 0);
        CHECK(area.h > 0);
        CHECK(area.x >= 0);
        CHECK(area.y >= 0);

        // Render shapes and text onto the figure's canvas
        fill_rect(fig.canvas(), area.x + 10, area.y + 10, 50, 50, Color{255, 0, 0});
        draw_circle(fig.canvas(), area.x + 100, area.y + 100, 20, Color{0, 255, 0});
        draw_text(fig.canvas(), area.x + 200, area.y + 50, "INTEGRATION TEST", Color{0, 0, 0}, 1);

        // Save figure to disk as a BMP image file
        fig.save(test_output_path);
    }

    // 2. Read back generated BMP file and validate binary format and headers
    {
        std::vector<std::uint8_t> bytes = read_binary_file(test_output_path);
        
        // BMP file must exist and contain at least 54 header bytes
        CHECK(bytes.size() >= 54);

        // Check 14-byte BITMAPFILEHEADER signature ('B', 'M')
        CHECK_EQ(bytes[0], static_cast<std::uint8_t>('B'));
        CHECK_EQ(bytes[1], static_cast<std::uint8_t>('M'));

        std::uint32_t file_size = read_u32_le(bytes, 2);
        CHECK_EQ(file_size, static_cast<std::uint32_t>(bytes.size()));

        std::uint32_t off_bits = read_u32_le(bytes, 10);
        CHECK_EQ(off_bits, 54u); // 14-byte file header + 40-byte info header

        // Check 40-byte BITMAPINFOHEADER
        std::uint32_t header_size = read_u32_le(bytes, 14);
        CHECK_EQ(header_size, 40u);

        std::uint32_t bmp_width = read_u32_le(bytes, 18);
        std::uint32_t bmp_height = read_u32_le(bytes, 22);
        CHECK_EQ(bmp_width, 640u);
        CHECK_EQ(bmp_height, 480u);

        std::uint16_t planes = read_u16_le(bytes, 26);
        CHECK_EQ(planes, 1u);

        std::uint16_t bpp = read_u16_le(bytes, 28);
        CHECK_EQ(bpp, 24u); // 24 bits per pixel (BGR)

        // Clean up temporary test file
        std::remove(test_output_path.c_str());
    }

    // 3. Test Figure margin adjustments
    {
        Figure fig(500, 400, theme_white());
        Margins custom_margins{50, 50, 30, 30};
        CHECK_NOTHROW(fig.set_margins(custom_margins));

        const Rect& area = fig.axes().area();
        CHECK_EQ(area.x, 50);
        CHECK_EQ(area.y, 30);
        CHECK_EQ(area.w, 500 - 50 - 50); // 400
        CHECK_EQ(area.h, 400 - 30 - 30); // 340
    }

    // 4. Test Figure move semantics (move construction and move assignment)
    {
        Figure fig1(300, 200, theme_white());
        fill_rect(fig1.canvas(), 0, 0, 10, 10, Color{123, 45, 67});

        // Move construct fig2 from fig1
        Figure fig2 = std::move(fig1);
        CHECK_EQ(fig2.canvas().width(), 300);
        CHECK_EQ(fig2.canvas().height(), 200);
        const Color expected_pixel{123, 45, 67};
        CHECK(fig2.canvas().get_pixel(5, 5) == expected_pixel);

        // Move assign fig3 from fig2
        Figure fig3(300, 200);
        fig3 = std::move(fig2);
        CHECK_EQ(fig3.canvas().width(), 300);
        CHECK_EQ(fig3.canvas().height(), 200);
        CHECK(fig3.canvas().get_pixel(5, 5) == expected_pixel);
    }

    return finish();
}
