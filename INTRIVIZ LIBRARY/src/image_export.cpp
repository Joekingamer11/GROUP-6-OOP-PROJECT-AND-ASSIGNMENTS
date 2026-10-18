#include "cppviz/image_export.hpp"
#include "cppviz/error.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace cppviz {

namespace {

// Writes a 16-bit unsigned integer in little-endian byte order.
void write_u16_le(std::ofstream& out, std::uint16_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

// Writes a 32-bit unsigned integer in little-endian byte order.
void write_u32_le(std::ofstream& out, std::uint32_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
    out.put(static_cast<char>((value >> 16) & 0xFF));
    out.put(static_cast<char>((value >> 24) & 0xFF));
}

// Extracts and normalizes the file extension from a path to lowercase.
std::string extract_lowercase_extension(const std::string& path) {
    auto dot_pos = path.rfind('.');
    if (dot_pos == std::string::npos || dot_pos == path.size() - 1) {
        return "";
    }
    std::string ext = path.substr(dot_pos);
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return ext;
}

} // anonymous namespace

void write_ppm(const Canvas& canvas, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw IoError("Failed to open file for writing PPM image: " + path);
    }

    int w = canvas.width();
    int h = canvas.height();

    // Netpbm P6 ASCII header
    out << "P6\n" << w << " " << h << "\n255\n";
    if (!out) {
        throw IoError("Failed to write PPM header to file: " + path);
    }

    // Raw RGB pixel payload (top-to-bottom scanlines)
    const auto& pixels = canvas.data();
    if (!pixels.empty()) {
        out.write(reinterpret_cast<const char*>(pixels.data()),
                  static_cast<std::streamsize>(pixels.size()));
    }

    if (!out) {
        throw IoError("Failed to write PPM pixel data to file: " + path);
    }
}

void write_bmp(const Canvas& canvas, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw IoError("Failed to open file for writing BMP image: " + path);
    }

    int w = canvas.width();
    int h = canvas.height();

    // BMP scanlines must be padded with zero bytes to a multiple of 4 bytes
    std::uint32_t raw_row_bytes = static_cast<std::uint32_t>(w * 3);
    std::uint32_t row_padding = (4 - (raw_row_bytes % 4)) % 4;
    std::uint32_t row_stride = raw_row_bytes + row_padding;
    std::uint32_t image_data_size = row_stride * static_cast<std::uint32_t>(h);

    constexpr std::uint32_t file_header_size = 14;
    constexpr std::uint32_t info_header_size = 40;
    constexpr std::uint32_t total_header_size = file_header_size + info_header_size; // 54 bytes
    std::uint32_t file_size = total_header_size + image_data_size;

    // 1. BITMAPFILEHEADER (14 bytes)
    out.put('B');
    out.put('M');
    write_u32_le(out, file_size);
    write_u16_le(out, 0); // bfReserved1
    write_u16_le(out, 0); // bfReserved2
    write_u32_le(out, total_header_size); // bfOffBits (54)

    // 2. BITMAPINFOHEADER (40 bytes)
    write_u32_le(out, info_header_size);              // biSize (40)
    write_u32_le(out, static_cast<std::uint32_t>(w)); // biWidth
    write_u32_le(out, static_cast<std::uint32_t>(h)); // biHeight (positive = bottom-up DIB)
    write_u16_le(out, 1);                             // biPlanes
    write_u16_le(out, 24);                            // biBitCount (24-bit BGR)
    write_u32_le(out, 0);                             // biCompression (0 = BI_RGB uncompressed)
    write_u32_le(out, image_data_size);               // biSizeImage
    write_u32_le(out, 0);                             // biXPelsPerMeter
    write_u32_le(out, 0);                             // biYPelsPerMeter
    write_u32_le(out, 0);                             // biClrUsed
    write_u32_le(out, 0);                             // biClrImportant

    if (!out) {
        throw IoError("Failed to write BMP headers to file: " + path);
    }

    // 3. Pixel Data (bottom scanline first, BGR byte order, padded to 4-byte boundary)
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            Color c = canvas.get_pixel(x, y);
            out.put(static_cast<char>(c.b));
            out.put(static_cast<char>(c.g));
            out.put(static_cast<char>(c.r));
        }
        for (std::uint32_t p = 0; p < row_padding; ++p) {
            out.put(0);
        }
    }

    if (!out) {
        throw IoError("Failed to write BMP pixel data to file: " + path);
    }
}

void save_image(const Canvas& canvas, const std::string& path) {
    std::string ext = extract_lowercase_extension(path);

    if (ext == ".ppm") {
        write_ppm(canvas, path);
    } else if (ext == ".bmp") {
        write_bmp(canvas, path);
    } else {
        throw InvalidArgument(
            "Unsupported image extension in path: '" + path +
            "'. Supported extensions in Week 1: .ppm, .bmp"
        );
    }
}

} // namespace cppviz