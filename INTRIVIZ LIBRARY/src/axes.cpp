/**
 * @file axes.cpp
 * @brief Implementation of plot coordinate system, scaling, and frame rendering (Axes).
 *
 * Axes manages the Cartesian rectangular plotting area, maps data-space coordinates
 * to pixel-space coordinates using attached X and Y scales, and renders background fills,
 * grid lines, axis spines, tick marks, and tick labels.
 */

#include "intriviz/axes.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/error.hpp"
#include "intriviz/font.hpp"
#include <cmath>

namespace intriviz {

/**
 * @brief Constructs an Axes within a given bounding rectangle and theme on a target Canvas.
 * 
 * @param canvas Reference to the Canvas on which this Axes will draw.
 * @param area Rectangular plot bounds in pixel coordinates.
 * @param theme Styling and visual theme configuration.
 */
Axes::Axes(Canvas& canvas, Rect area, const Theme& theme)
    : canvas_(&canvas), area_(area), theme_(theme) {}

/**
 * @brief Returns the pixel bounding box of the active plot area.
 * 
 * @return Const reference to the Rect defining the plot region.
 */
const Rect& Axes::area() const {
    return area_;
}

/**
 * @brief Updates the pixel bounding box of the plot area.
 * 
 * @param area New bounding rectangle.
 */
void Axes::set_area(const Rect& area) {
    area_ = area;
}

/**
 * @brief Returns a mutable reference to the Canvas associated with this Axes.
 * 
 * @return Mutable reference to Canvas.
 */
Canvas& Axes::canvas() {
    return *canvas_;
}

/**
 * @brief Returns the visual theme configuration applied to this Axes.
 * 
 * @return Const reference to Theme.
 */
const Theme& Axes::theme() const {
    return theme_;
}

/**
 * @brief Computes the horizontal pixel interval {left, right} for the plot area.
 * 
 * @return Pair of pixel coordinates {x_min, x_max}.
 */
std::pair<double, double> Axes::x_pixel_range() const {
    double left = area_.x;
    double right = area_.x + area_.w;
    return {left, right};
}

/**
 * @brief Computes the vertical pixel interval {bottom, top} for the plot area.
 * 
 * In screen coordinates, pixel Y increases downward; therefore, the bottom of the
 * plot corresponds to area.y + area.h and the top corresponds to area.y.
 * 
 * @return Pair of pixel coordinates {y_bottom, y_top}.
 */
std::pair<double, double> Axes::y_pixel_range() const {
    // In screen coordinates, pixel y increases downward.
    // Bottom of the plot area corresponds to area_.y + area_.h
    // Top of the plot area corresponds to area_.y
    double bottom = area_.y + area_.h;
    double top = area_.y;
    return {bottom, top};
}

/**
 * @brief Assigns a continuous or categorical Scale to the horizontal (X) axis.
 * 
 * @param scale Shared pointer to the configured Scale.
 */
void Axes::set_x_scale(std::shared_ptr<Scale> scale) {
    x_scale_ = std::move(scale);
}

/**
 * @brief Assigns a continuous or categorical Scale to the vertical (Y) axis.
 * 
 * @param scale Shared pointer to the configured Scale.
 */
void Axes::set_y_scale(std::shared_ptr<Scale> scale) {
    y_scale_ = std::move(scale);
}

/**
 * @brief Retrieves the active Scale for the X axis.
 * 
 * @return Const reference to the X Scale.
 * @throws InvalidArgument If the X scale has not been set.
 */
const Scale& Axes::x_scale() const {
    if (!x_scale_) {
        throw InvalidArgument("X scale has not been set on Axes");
    }
    return *x_scale_;
}

/**
 * @brief Retrieves the active Scale for the Y axis.
 * 
 * @return Const reference to the Y Scale.
 * @throws InvalidArgument If the Y scale has not been set.
 */
const Scale& Axes::y_scale() const {
    if (!y_scale_) {
        throw InvalidArgument("Y scale has not been set on Axes");
    }
    return *y_scale_;
}

/**
 * @brief Converts a 2D data-space coordinate (x, y) into integer pixel coordinates.
 * 
 * Uses the attached X and Y scales to map data values to pixels, rounding to the nearest integer.
 * 
 * @param x Coordinate in data space along the X domain.
 * @param y Coordinate in data space along the Y domain.
 * @return PointI containing integer pixel coordinates {px, py}.
 */
PointI Axes::to_pixel(double x, double y) const {
    const Scale& xs = x_scale();
    const Scale& ys = y_scale();
    int px = static_cast<int>(std::round(xs.map(x)));
    int py = static_cast<int>(std::round(ys.map(y)));
    return PointI{px, py};
}

/**
 * @brief Renders the visual axis frame including background, grid, spines, ticks, and labels.
 * 
 * Execution stages:
 * 1. Fills the inner plot area with theme.plot_background.
 * 2. If theme.show_grid is true, draws vertical and horizontal grid lines at tick locations.
 * 3. Draws the left vertical spine and bottom horizontal spine.
 * 4. Renders downward X-axis tick marks and centered tick text labels.
 * 5. Renders leftward Y-axis tick marks and right-aligned tick text labels.
 */
void Axes::draw_frame() {
    // 1. Fill plot background area
    fill_rect(*canvas_, area_.x, area_.y, area_.w, area_.h, theme_.plot_background);

    // 2. Draw grid lines if enabled
    if (theme_.show_grid) {
        if (x_scale_) {
            for (double v : x_scale_->tick_values()) {
                int px = static_cast<int>(std::round(x_scale_->map(v)));
                if (px >= area_.x && px <= area_.x + area_.w) {
                    draw_line(*canvas_, px, area_.y, px, area_.y + area_.h, theme_.grid_color, 1);
                }
            }
        }
        if (y_scale_) {
            for (double v : y_scale_->tick_values()) {
                int py = static_cast<int>(std::round(y_scale_->map(v)));
                if (py >= area_.y && py <= area_.y + area_.h) {
                    draw_line(*canvas_, area_.x, py, area_.x + area_.w, py, theme_.grid_color, 1);
                }
            }
        }
    }

    // 3. Draw axis frame lines (left and bottom)
    draw_line(*canvas_, area_.x, area_.y, area_.x, area_.y + area_.h, theme_.axis_color, theme_.line_thickness);
    draw_line(*canvas_, area_.x, area_.y + area_.h, area_.x + area_.w, area_.y + area_.h, theme_.axis_color, theme_.line_thickness);

    // 4. Draw X ticks and labels
    if (x_scale_) {
        for (double v : x_scale_->tick_values()) {
            int px = static_cast<int>(std::round(x_scale_->map(v)));
            if (px >= area_.x && px <= area_.x + area_.w) {
                // 5-pixel tick mark extending downwards
                draw_line(*canvas_, px, area_.y + area_.h, px, area_.y + area_.h + 5, theme_.axis_color, 1);

                std::string label = x_scale_->tick_label(v);
                int tw = text_width(label, theme_.font_scale);
                int tx = px - tw / 2; // Centered under tick
                int ty = area_.y + area_.h + 8;
                draw_text(*canvas_, tx, ty, label, theme_.text_color, theme_.font_scale);
            }
        }
    }

    // 5. Draw Y ticks and labels
    if (y_scale_) {
        for (double v : y_scale_->tick_values()) {
            int py = static_cast<int>(std::round(y_scale_->map(v)));
            if (py >= area_.y && py <= area_.y + area_.h) {
                // 5-pixel tick mark extending leftwards
                draw_line(*canvas_, area_.x - 5, py, area_.x, py, theme_.axis_color, 1);

                std::string label = y_scale_->tick_label(v);
                int tw = text_width(label, theme_.font_scale);
                int th = text_height(theme_.font_scale);
                int tx = area_.x - 8 - tw; // Right-aligned to the left of tick mark
                int ty = py - th / 2;      // Vertically centered
                draw_text(*canvas_, tx, ty, label, theme_.text_color, theme_.font_scale);
            }
        }
    }
}

} // namespace intriviz
