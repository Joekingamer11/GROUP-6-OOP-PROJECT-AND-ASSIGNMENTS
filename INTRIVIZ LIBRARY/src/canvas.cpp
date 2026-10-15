#include "cppviz/canvas.hpp"
#include "cppviz/error.hpp"

#include <cstddef>
#include <string>

namespace cppviz {

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

int Canvas::width() const {
    return w_;
}

int Canvas::height() const {
    return h_;
}

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

void Canvas::fill(Color c) {
    std::size_t total_pixels = static_cast<std::size_t>(w_) * static_cast<std::size_t>(h_);
    for (std::size_t i = 0; i < total_pixels; ++i) {
        std::size_t idx = i * 3;
        pixels_[idx]     = c.r;
        pixels_[idx + 1] = c.g;
        pixels_[idx + 2] = c.b;
    }
}

const std::vector<std::uint8_t>& Canvas::data() const {
    return pixels_;
}

} // namespace cppviz
