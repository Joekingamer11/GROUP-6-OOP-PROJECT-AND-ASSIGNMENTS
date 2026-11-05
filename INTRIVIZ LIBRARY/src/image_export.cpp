/**
 * @file image_export.cpp
 * @brief Implementation of image file writers (PPM, BMP, PNG) and format dispatcher.
 *
 * Implements pure C++ image encoders without external library dependencies:
 * - Netpbm PPM (P6 binary RGB)
 * - Microsoft Windows Bitmap (24-bit uncompressed BMP with 4-byte row padding)
 * - Portable Network Graphics (PNG with Deflate uncompressed blocks, Adler-32, and CRC-32)
 */

#include "intriviz/image_export.hpp"
#include "intriviz/error.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace intriviz {

namespace {

// ============================================================================
// Endian & Bit Manipulation Helpers
// ============================================================================

/**
 * @brief Writes a 16-bit unsigned integer in little-endian byte order (used in BMP headers).
 * 
 * @param out Output file stream.
 * @param value The 16-bit value to write.
 */
void write_u16_le(std::ofstream& out, std::uint16_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

/**
 * @brief Writes a 32-bit unsigned integer in little-endian byte order (used in BMP headers).
 * 
 * @param out Output file stream.
 * @param value The 32-bit value to write.
 */
void write_u32_le(std::ofstream& out, std::uint32_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
    out.put(static_cast<char>((value >> 16) & 0xFF));
    out.put(static_cast<char>((value >> 24) & 0xFF));
}

/**
 * @brief Appends a 32-bit unsigned integer in big-endian byte order to a byte buffer (used in PNG chunks).
 * 
 * @param buf Target byte buffer.
 * @param value The 32-bit integer to append.
 */
void append_u32_be(std::vector<std::uint8_t>& buf, std::uint32_t value) {
    buf.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    buf.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    buf.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    buf.push_back(static_cast<std::uint8_t>(value & 0xFF));
}

/**
 * @brief Extracts the file extension from a path string and converts it to lowercase.
 * 
 * @param path File system path string.
 * @return Lowercase extension including the leading dot (e.g., ".png"), or empty string if none.
 */
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

// ============================================================================
// Checksum Algorithms for PNG (CRC-32 & Adler-32)
// ============================================================================

/**
 * @brief Computes standard IEEE 802.3 CRC-32 polynomial checksum over a byte range.
 * 
 * Uses a precomputed 256-entry lookup table for high-performance chunk verification in PNG files.
 * 
 * @param data Pointer to input byte sequence.
 * @param len Number of bytes.
 * @return 32-bit CRC checksum.
 */
std::uint32_t compute_crc32(const std::uint8_t* data, std::size_t len) {
    static bool table_computed = false;
    // FIXED BUG: Changed to an array of size 256
    static std::uint32_t crc_table[256]; 

    if (!table_computed) {
        for (std::uint32_t n = 0; n < 256; ++n) {
            std::uint32_t c = n;
            for (int k = 0; k < 8; ++k) {
                if (c & 1) {
                    c = 0xEDB88320L ^ (c >> 1);
                } else {
                    c = c >> 1;
                }
            }
            crc_table[n] = c;
        }
        table_computed = true;
    }

    std::uint32_t c = 0xFFFFFFFFL;
    for (std::size_t i = 0; i < len; ++i) {
        c = crc_table[(c ^ data[i]) & 0xFF] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFFL;
}

/**
 * @brief Computes the zlib Adler-32 checksum required at the tail of zlib streams in PNG IDAT chunks.
 * 
 * @param data Pointer to input byte sequence.
 * @param len Number of bytes.
 * @return 32-bit Adler-32 checksum.
 */
std::uint32_t compute_adler32(const std::uint8_t* data, std::size_t len) {
    constexpr std::uint32_t MOD_ADLER = 65521;
    std::uint32_t a = 1;
    std::uint32_t b = 0;

    for (std::size_t i = 0; i < len; ++i) {
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    return (b << 16) | a;
}

/**
 * @brief Formats and writes an individual PNG chunk to a file stream.
 * 
 * Writes: 4-byte length + 4-byte chunk type + payload data + 4-byte CRC-32 over (type + payload).
 * 
 * @param out Target output file stream.
 * @param type 4-character ASCII chunk type tag (e.g. "IHDR", "IDAT", "IEND").
 * @param payload Binary byte buffer containing chunk-specific data.
 */
void write_png_chunk(std::ofstream& out, const char type[5], const std::vector<std::uint8_t>& payload) {
    std::uint32_t length = static_cast<std::uint32_t>(payload.size());
    std::vector<std::uint8_t> chunk_buf;
    chunk_buf.reserve(8 + payload.size());

    // Chunk Type (4 bytes)
    for (int i = 0; i < 4; ++i) {
        chunk_buf.push_back(static_cast<std::uint8_t>(type[i]));
    }
    // Chunk Payload
    chunk_buf.insert(chunk_buf.end(), payload.begin(), payload.end());

    // CRC-32 calculation over Chunk Type + Payload
    std::uint32_t crc = compute_crc32(chunk_buf.data(), chunk_buf.size());

    // Write Length (big-endian 4 bytes)
    std::vector<std::uint8_t> len_bytes;
    append_u32_be(len_bytes, length);
    out.write(reinterpret_cast<const char*>(len_bytes.data()), 4);

    // Write Type + Payload
    out.write(reinterpret_cast<const char*>(chunk_buf.data()), static_cast<std::streamsize>(chunk_buf.size()));

    // Write CRC-32 (big-endian 4 bytes)
    std::vector<std::uint8_t> crc_bytes;
    append_u32_be(crc_bytes, crc);
    out.write(reinterpret_cast<const char*>(crc_bytes.data()), 4);
}

} // anonymous namespace

// ============================================================================
// Netpbm PPM (P6 Binary) Exporter
// ============================================================================

/**
 * @brief Exports a Canvas image to disk in binary PPM (P6) format.
 * 
 * Writes standard "P6\n<w> <h>\n255\n" ASCII header followed by raw binary RGB bytes.
 * 
 * @param canvas The source Canvas.
 * @param path Destination file path.
 * @throws IoError If opening or writing to the file fails.
 */
void write_ppm(const Canvas& canvas, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw IoError("Failed to open file for writing PPM image: " + path);
    }

    int w = canvas.width();
    int h = canvas.height();

    // ASCII Netpbm Header
    out << "P6\n" << w << " " << h << "\n255\n";
    if (!out) {
        throw IoError("Failed to write PPM header to file: " + path);
    }

    // Binary RGB scanlines
    const auto& pixels = canvas.data();
    if (!pixels.empty()) {
        out.write(reinterpret_cast<const char*>(pixels.data()),
                  static_cast<std::streamsize>(pixels.size()));
    }

    if (!out) {
        throw IoError("Failed to write PPM pixel data to file: " + path);
    }
}

// ============================================================================
// Windows 24-bit Uncompressed BMP Exporter
// ============================================================================

/**
 * @brief Exports a Canvas image to disk as a 24-bit uncompressed Windows BMP file.
 * 
 * Generates standard 14-byte BITMAPFILEHEADER and 40-byte BITMAPINFOHEADER structures,
 * and writes bottom-up scanlines in BGR byte order with 4-byte row padding.
 * 
 * @param canvas The source Canvas.
 * @param path Destination file path.
 * @throws IoError If opening or writing to the file fails.
 */
void write_bmp(const Canvas& canvas, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw IoError("Failed to open file for writing BMP image: " + path);
    }

    int w = canvas.width();
    int h = canvas.height();

    std::uint32_t raw_row_bytes = static_cast<std::uint32_t>(w * 3);
    std::uint32_t row_padding = (4 - (raw_row_bytes % 4)) % 4;
    std::uint32_t row_stride = raw_row_bytes + row_padding;
    std::uint32_t image_data_size = row_stride * static_cast<std::uint32_t>(h);

    constexpr std::uint32_t file_header_size = 14;
    constexpr std::uint32_t info_header_size = 40;
    constexpr std::uint32_t total_header_size = file_header_size + info_header_size; // 54
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
    write_u32_le(out, static_cast<std::uint32_t>(h)); // biHeight (bottom-up)
    write_u16_le(out, 1);                             // biPlanes
    write_u16_le(out, 24);                            // biBitCount (24-bit BGR)
    write_u32_le(out, 0);                             // biCompression (0 = BI_RGB)
    write_u32_le(out, image_data_size);               // biSizeImage
    write_u32_le(out, 0);                             // biXPelsPerMeter
    write_u32_le(out, 0);                             // biYPelsPerMeter
    write_u32_le(out, 0);                             // biClrUsed
    write_u32_le(out, 0);                             // biClrImportant

    if (!out) {
        throw IoError("Failed to write BMP headers to file: " + path);
    }

    // 3. Pixel Data (bottom-up scanlines, BGR byte order, padded to 4 bytes)
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

// ============================================================================
// Portable Network Graphics (PNG) Exporter
// ============================================================================

/**
 * @brief Exports a Canvas image to disk as a Portable Network Graphics (PNG) file.
 * 
 * Implements a pure C++ PNG writer:
 * - Emits the 8-byte PNG file magic header
 * - Writes the IHDR chunk (24-bit RGB, Truecolor)
 * - Encodes raw scanlines with filter type 0 inside zlib uncompressed DEFLATE blocks (IDAT chunk)
 * - Emits the terminal IEND chunk
 * 
 * @param canvas The source Canvas.
 * @param path Destination file path.
 * @throws IoError If file stream operations fail.
 */
void write_png(const Canvas& canvas, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw IoError("Failed to open file for writing PNG image: " + path);
    }

    int w = canvas.width();
    int h = canvas.height();

    // 1. PNG 8-byte Magic Signature
    // FIXED BUG: Changed array size from 6 to 8
    constexpr std::uint8_t png_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    out.write(reinterpret_cast<const char*>(png_signature), 8);

    // 2. IHDR Chunk (Image Header)
    std::vector<std::uint8_t> ihdr_payload;
    append_u32_be(ihdr_payload, static_cast<std::uint32_t>(w)); // Width
    append_u32_be(ihdr_payload, static_cast<std::uint32_t>(h)); // Height
    ihdr_payload.push_back(8); // Bit depth (8 bits per channel)
    ihdr_payload.push_back(2); // Color type (2 = Truecolor RGB)
    ihdr_payload.push_back(0); // Compression method (0 = DEFLATE)
    ihdr_payload.push_back(0); // Filter method (0 = Adaptive filtering)
    ihdr_payload.push_back(0); // Interlace method (0 = No interlace)
    write_png_chunk(out, "IHDR", ihdr_payload);

    // 3. IDAT Chunk (Raw Uncompressed Scanlines inside Zlib Container)
    std::vector<std::uint8_t> raw_scanlines;
    raw_scanlines.reserve(static_cast<std::size_t>(h) * (1 + static_cast<std::size_t>(w) * 3));

    for (int y = 0; y < h; ++y) {
        raw_scanlines.push_back(0); // Filter type 0 (None)
        for (int x = 0; x < w; ++x) {
            Color c = canvas.get_pixel(x, y);
            raw_scanlines.push_back(c.r);
            raw_scanlines.push_back(c.g);
            raw_scanlines.push_back(c.b);
        }
    }

    std::uint32_t adler = compute_adler32(raw_scanlines.data(), raw_scanlines.size());

    // Zlib Stream Construction
    std::vector<std::uint8_t> zlib_stream;
    zlib_stream.push_back(0x78); // CMF (Deflate, 32K window)
    zlib_stream.push_back(0x01); // FLG (FCHECK = 1, Default compression)

    // Uncompressed DEFLATE Blocks (max 65535 uncompressed bytes per block)
    std::size_t remaining = raw_scanlines.size();
    std::size_t offset = 0;

    while (remaining > 0) {
        std::uint16_t block_len = static_cast<std::uint16_t>(std::min<std::size_t>(remaining, 65535));
        remaining -= block_len;

        std::uint8_t bfinal_btype = (remaining == 0) ? 0x01 : 0x00; // Final block flag + BTYPE 00
        zlib_stream.push_back(bfinal_btype);

        // LEN and NLEN (little-endian 16-bit block length and bitwise complement)
        zlib_stream.push_back(static_cast<std::uint8_t>(block_len & 0xFF));
        zlib_stream.push_back(static_cast<std::uint8_t>((block_len >> 8) & 0xFF));

        std::uint16_t nlen = ~block_len;
        zlib_stream.push_back(static_cast<std::uint8_t>(nlen & 0xFF));
        zlib_stream.push_back(static_cast<std::uint8_t>((nlen >> 8) & 0xFF));

        // Block payload bytes
        zlib_stream.insert(zlib_stream.end(),
                           raw_scanlines.begin() + offset,
                           raw_scanlines.begin() + offset + block_len);
        offset += block_len;
    }

    // Append Zlib Adler-32 Checksum (big-endian)
    append_u32_be(zlib_stream, adler);

    write_png_chunk(out, "IDAT", zlib_stream);

    // 4. IEND Chunk (End of PNG Stream)
    write_png_chunk(out, "IEND", {});

    if (!out) {
        throw IoError("Failed to write PNG chunks to file: " + path);
    }
}

// ============================================================================
// Automatic Extension Dispatcher
// ============================================================================

/**
 * @brief Saves a Canvas image to disk, automatically selecting the format from the file extension.
 * 
 * Supports ".ppm", ".bmp", and ".png" extensions (case-insensitive).
 * 
 * @param canvas The Canvas to export.
 * @param path Destination file path.
 * @throws InvalidArgument If the extension is unsupported or missing.
 * @throws IoError If file writing fails.
 */
void save_image(const Canvas& canvas, const std::string& path) {
    std::string ext = extract_lowercase_extension(path);

    if (ext == ".ppm") {
        write_ppm(canvas, path);
    } else if (ext == ".bmp") {
        write_bmp(canvas, path);
    } else if (ext == ".png") {
        write_png(canvas, path);
    } else {
        throw InvalidArgument(
            "Unsupported image extension in path: '" + path +
            "'. Supported extensions: .ppm, .bmp, .png"
        );
    }
}

} // namespace intriviz
