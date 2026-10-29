#pragma once

#include <string>
#include "intriviz/canvas.hpp"

namespace intriviz {

/**
 * @brief Exports a Canvas to a Netpbm PPM (P6 binary) image file.
 * 
 * Writes an ASCII header ("P6\n<width> <height>\n255\n") followed by raw binary
 * RGB pixel bytes scanline-by-scanline from top to bottom.
 * 
 * @param canvas The Canvas instance containing pixel data.
 * @param path Destination filesystem path.
 * @throws IoError If the destination file cannot be opened or written.
 */
void write_ppm(const Canvas& canvas, const std::string& path);

/**
 * @brief Exports a Canvas to a 24-bit uncompressed Windows BMP image file.
 * 
 * Constructs standard 14-byte BITMAPFILEHEADER and 40-byte BITMAPINFOHEADER structures,
 * encodes scanlines bottom-up, converts RGB to BGR pixel byte order, and pads each
 * scanline to a 4-byte boundary.
 * 
 * @param canvas The Canvas instance containing pixel data.
 * @param path Destination filesystem path.
 * @throws IoError If the destination file cannot be opened or written.
 */
void write_bmp(const Canvas& canvas, const std::string& path);

/**
 * @brief Exports a Canvas to a 24-bit RGB Portable Network Graphics (PNG) file.
 * 
 * Generates a valid PNG stream containing an 8-byte PNG signature, IHDR chunk,
 * IDAT chunk with uncompressed DEFLATE blocks and zlib Adler-32 checksum, and IEND chunk.
 * Operates without external zlib/libpng dependencies.
 * 
 * @param canvas The Canvas instance containing pixel data.
 * @param path Destination filesystem path.
 * @throws IoError If the destination file cannot be opened or written.
 */
void write_png(const Canvas& canvas, const std::string& path);

/**
 * @brief Saves a Canvas to an image file, automatically determining the format by extension.
 * 
 * Inspects the file extension of path (case-insensitive). Supported extensions:
 * - ".ppm": Netpbm P6 Binary Image
 * - ".bmp": 24-bit Windows Bitmap
 * - ".png": 24-bit Portable Network Graphics
 * 
 * @param canvas The Canvas instance containing pixel data.
 * @param path Destination filesystem path.
 * @throws InvalidArgument If the extension is missing or unsupported.
 * @throws IoError If the destination file cannot be opened or written.
 */
void save_image(const Canvas& canvas, const std::string& path);

} // namespace intriviz