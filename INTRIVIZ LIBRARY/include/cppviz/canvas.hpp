#pragma once
#include <cstdint>
#include <vector>
#include "cppviz/types.hpp"

namespace cppviz {

/**
 * @brief Represents a 2D grid of RGB pixels stored in row-major order.
 * 
 * Top-left corner is at (0, 0). Pixel data is stored continuously as 24-bit RGB
 * (3 bytes per pixel) row-by-row, top row first.
 */
class Canvas {
public:
    /**
     * @brief Constructs a new Canvas with the specified dimensions and background color.
     * @param width Positive canvas width in pixels.
     * @param height Positive canvas height in pixels.
     * @param background Initial color for all pixels (default: white {255, 255, 255}).
     * @throws InvalidArgument If width <= 0 or height <= 0.
     */
    Canvas(int width, int height, Color background = Color{255, 255, 255});

    /// Returns the canvas width in pixels.
    int width() const;

    /// Returns the canvas height in pixels.
    int height() const;

    /**
     * @brief Sets the color of the pixel at coordinate (x, y).
     * 
     * Silently ignores coordinates outside the valid canvas bounds [0, width) x [0, height)
     * to facilitate safe shape drawing and boundary clipping.
     */
    void set_pixel(int x, int y, Color c);

    /**
     * @brief Retrieves the color of the pixel at coordinate (x, y).
     * @throws InvalidArgument If (x, y) is outside canvas bounds.
     */
    Color get_pixel(int x, int y) const;

    /**
     * @brief Fills the entire canvas with the specified color.
     */
    void fill(Color c);

    /**
     * @brief Provides read-only access to the underlying raw RGB byte buffer.
     * 
     * Buffer size is width * height * 3 bytes, ordered row-by-row from top to bottom.
     */
    const std::vector<std::uint8_t>& data() const;

private:
    int w_;
    int h_;
    std::vector<std::uint8_t> pixels_; // size = w * h * 3
};

} // namespace cppviz