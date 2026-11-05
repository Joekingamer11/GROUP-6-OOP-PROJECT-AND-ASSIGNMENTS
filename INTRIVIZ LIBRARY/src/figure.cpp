/**
 * @file figure.cpp
 * @brief Implementation of top-level Figure container and layout coordinator.
 *
 * A Figure owns a Canvas pixel buffer and a primary Axes object, coordinating
 * margins, layout bounds, themes, and image file export.
 */

#include "intriviz/figure.hpp"
#include "intriviz/error.hpp"
#include "intriviz/image_export.hpp"
#include <utility>

namespace intriviz {

namespace {

/**
 * @brief Computes the active plot rectangle by subtracting margins from canvas dimensions.
 * 
 * @param width Total canvas width.
 * @param height Total canvas height.
 * @param margins Inner padding margins around the plot area.
 * @return Rect defining the plot area.
 * @throws InvalidArgument If remaining plot dimensions are less than 10x10 pixels.
 */
Rect compute_axes_area(int width, int height, const Margins& margins) {
    int w = width - margins.left - margins.right;
    int h = height - margins.top - margins.bottom;
    require(w >= 10 && h >= 10, "Figure margins leave less than 10x10 pixels for plot area");
    return Rect{margins.left, margins.top, w, h};
}

} // anonymous namespace

/**
 * @brief Constructs a Figure with specified canvas dimensions and visual theme.
 * 
 * Allocates the underlying Canvas and initializes the primary Axes with default margins.
 * 
 * @param width Overall image width in pixels.
 * @param height Overall image height in pixels.
 * @param theme Color and styling theme configuration.
 */
Figure::Figure(int width, int height, const Theme& theme)
    : canvas_(std::make_unique<Canvas>(width, height, theme.background)),
      axes_(*canvas_, compute_axes_area(width, height, Margins{}), theme),
      margins_() {}

/**
 * @brief Returns a mutable reference to the primary plot Axes.
 * 
 * @return Mutable reference to Axes.
 */
Axes& Figure::axes() {
    return axes_;
}

/**
 * @brief Returns a constant reference to the primary plot Axes.
 * 
 * @return Read-only reference to Axes.
 */
const Axes& Figure::axes() const {
    return axes_;
}

/**
 * @brief Returns a mutable reference to the underlying pixel Canvas.
 * 
 * @return Mutable reference to Canvas.
 */
Canvas& Figure::canvas() {
    return *canvas_;
}

/**
 * @brief Returns a constant reference to the underlying pixel Canvas.
 * 
 * @return Read-only reference to Canvas.
 */
const Canvas& Figure::canvas() const {
    return *canvas_;
}

/**
 * @brief Retrieves the active visual Theme applied to this figure.
 * 
 * @return Const reference to the Theme structure.
 */
const Theme& Figure::theme() const {
    return axes_.theme();
}

/**
 * @brief Updates the figure margins and adjusts the inner Axes plot rectangle accordingly.
 * 
 * @param margins New margins specifying left, right, top, and bottom pixel clearances.
 */
void Figure::set_margins(const Margins& margins) {
    Rect new_area = compute_axes_area(canvas_->width(), canvas_->height(), margins);
    margins_ = margins;
    axes_.set_area(new_area);
}

/**
 * @brief Retrieves the current layout margins of the figure.
 * 
 * @return Reference to Margins struct.
 */
const Margins& Figure::margins() const {
    return margins_;
}

/**
 * @brief Saves the current canvas contents to an image file on disk.
 * 
 * Dispatches to PPM, BMP, or PNG exporters based on the file extension.
 * 
 * @param path Destination file path (e.g., "plot.png").
 */
void Figure::save(const std::string& path) const {
    save_image(*canvas_, path);
}

} // namespace intriviz
