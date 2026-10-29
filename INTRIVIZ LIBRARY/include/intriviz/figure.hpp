#pragma once

#include <memory>
#include <string>
#include "intriviz/axes.hpp"
#include "intriviz/canvas.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

/**
 * @brief High-level plot container owning a Canvas and managing its Axes area.
 * 
 * Figure owns the underlying Canvas on the heap via std::unique_ptr so that
 * when a Figure is moved, the Canvas memory address remains constant and
 * the internal Axes object continues to reference a valid Canvas pointer.
 */
class Figure {
public:
    /**
     * @brief Constructs a Figure with width, height, and theme.
     * @param width Canvas width in pixels (default: 640).
     * @param height Canvas height in pixels (default: 480).
     * @param theme Styling theme preset (default: theme_whitegrid()).
     * @throws InvalidArgument if remaining plot area is smaller than 10x10.
     */
    explicit Figure(int width = 640, int height = 480, const Theme& theme = theme_whitegrid());

    // Move-only semantics
    Figure(Figure&&) noexcept = default;
    Figure& operator=(Figure&&) noexcept = default;

    Figure(const Figure&) = delete;
    Figure& operator=(const Figure&) = delete;

    ~Figure() = default;

    /// Returns a reference to the main plot Axes.
    Axes& axes();
    const Axes& axes() const;

    /// Returns a reference to the underlying Canvas.
    Canvas& canvas();
    const Canvas& canvas() const;

    /// Returns the active theme.
    const Theme& theme() const;

    /**
     * @brief Recomputes the Axes plot area given new margins.
     * @param margins New plot margins.
     * @throws InvalidArgument if resulting plot area width or height is < 10.
     */
    void set_margins(const Margins& margins);

    /// Returns the current margins.
    const Margins& margins() const;

    /**
     * @brief Saves the canvas to disk in BMP or PPM format.
     * @param path Target file output path (.bmp or .ppm).
     * @throws InvalidArgument if path extension is unsupported.
     * @throws IoError if target file cannot be created.
     */
    void save(const std::string& path) const;

private:
    std::unique_ptr<Canvas> canvas_; // Must be declared BEFORE axes_
    Axes axes_;
    Margins margins_;
};

} // namespace intriviz