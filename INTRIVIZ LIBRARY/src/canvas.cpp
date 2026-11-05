/**
 * @file canvas.cpp
 * @brief Implementation of in-memory 24-bit RGB pixel buffer (Canvas).
 *
 * Provides the foundational drawing surface for all Intriviz graphics,
 * maintaining a contiguous RGB byte buffer with bounds-checked access and clipping.
 */

#include "intriviz/canvas.hpp"
#include "intriviz/error.hpp"

#include <cstddef>
#include <string>

namespace intriviz {

/**
 * @brief Constructs a Canvas of specified dimensions and clears it with a background color.
 * 
 * @param width Canvas width in pixels (must be > 0).
 * @param height Canvas height in pixels (must be > 0).
 * @param background Initial background color to fill the canvas with.
 * @throws InvalidArgument If width <= 0 or height <= 0.
 */
Canvas::Canvas(int width, int height, Color background)
    : w_(width), h_(height) {
    if (width <= 0 || height <= 0) {
        throw InvalidArgument(
            "Canvas dimensions must be positive, got width=" +
            std::to_string(width) + ", height=" + std::to_string(height)
        );
    }

    pixels_.resize(static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_) * 3);
    fill(background);
}

/**
 * @brief Returns the canvas width in pixels.
 * 
 * @return Width of the pixel buffer.
 */
int Canvas::width() const {
    return w_;
}

/**
 * @brief Returns the canvas height in pixels.
 * 
 * @return Height of the pixel buffer.
 */
int Canvas::height() const {
    return h_;
}

/**
 * @brief Sets the RGB color of an individual pixel at coordinates (x, y).
 * 
 * Out-of-bounds coordinates are silently ignored to provide automatic clipping
 * for rendering primitives.
 * 
 * @param x Pixel x-coordinate (0 = leftmost).
 * @param y Pixel y-coordinate (0 = topmost).
 * @param c Color to assign to the pixel.
 */
void Canvas::set_pixel(int x, int y, Color c) {
    if (x < 0 || x >= w_ || y < 0 || y >= h_) {
        // Silently ignore out-of-bounds drawing to enable effortless clipping
        return;
    }
    std::size_t idx = (static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) +
                       static_cast<std::size_t>(x)) * 3;
    pixels_[idx]     = c.r;
    pixels_[idx + 1] = c.g;
    pixels_[idx + 2] = c.b;
}

/**
 * @brief Retrieves the color of the pixel at coordinates (x, y).
 * 
 * @param x Pixel x-coordinate.
 * @param y Pixel y-coordinate.
 * @return The Color at (x, y).
 * @throws InvalidArgument If (x, y) is out of bounds.
 */
Color Canvas::get_pixel(int x, int y) const {
    if (x < 0 || x >= w_ || y < 0 || y >= h_) {
        throw InvalidArgument(
            "Pixel coordinates (" + std::to_string(x) + ", " + std::to_string(y) +
            ") out of bounds for canvas size " + std::to_string(w_) + "x" + std::to_string(h_)
        );
    }
    std::size_t idx = (static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) +
                       static_cast<std::size_t>(x)) * 3;
    return Color{pixels_[idx], pixels_[idx + 1], pixels_[idx + 2]};
}

/**
 * @brief Fills the entire canvas buffer with a uniform color.
 * 
 * @param c The fill Color.
 */
void Canvas::fill(Color c) {
    std::size_t total_pixels = static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_);
    for (std::size_t i = 0; i < total_pixels; ++i) {
        std::size_t idx = i * 3;
        pixels_[idx]     = c.r;
        pixels_[idx + 1] = c.g;
        pixels_[idx + 2] = c.b;
    }
}

/**
 * @brief Provides direct read-only access to the underlying raw RGB byte buffer.
 * 
 * @return Constant reference to vector of interleaved RGB bytes (size = width * height * 3).
 */
const std::vector<std::uint8_t>& Canvas::data() const {
    return pixels_;
}

} // namespace intriviz
