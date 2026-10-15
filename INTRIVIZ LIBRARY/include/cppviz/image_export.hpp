#pragma once

#include <string>
#include "cppviz/canvas.hpp"

namespace cppviz {

/**
 * @brief Exports a Canvas to a Netpbm PPM (P6 binary) image file.
 * 
 * Writes an ASCII header ("P6\n<width> <height>\n255\n") followed by raw binary
 * RGB pixel data row-by-row from top to bottom.
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
 * @brief Saves a Canvas to an image file, determining the format by file extension.
 * 
 * Inspects the file extension of path (case-insensitive). Week 1 supports ".ppm"
 * and ".bmp". Week 4 adds support for ".png".
 * 
 * @param canvas The Canvas instance containing pixel data.
 * @param path Destination filesystem path.
 * @throws InvalidArgument If the extension is missing or unsupported.
 * @throws IoError If the destination file cannot be opened or written.
 */
void save_image(const Canvas& canvas, const std::string& path);

} // namespace cppviz