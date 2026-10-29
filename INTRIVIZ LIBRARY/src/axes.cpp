#include "cppviz/axes.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/error.hpp"
#include "cppviz/font.hpp"
#include <cmath>

namespace cppviz {

Axes::Axes(Canvas& canvas, Rect area, const Theme& theme)
    : canvas_(&canvas), area_(area), theme_(theme) {}

const Rect& Axes::area() const {
    return area_;
}

void Axes::set_area(const Rect& area) {
    area_ = area;
}

Canvas& Axes::canvas() {
    return *canvas_;
}

const Theme& Axes::theme() const {
    return theme_;
}

std::pair<double, double> Axes::x_pixel_range() const {
    double left = area_.x;
    double right = area_.x + area_.w;
    return {left, right};
}

std::pair<double, double> Axes::y_pixel_range() const {
    // In screen coordinates, pixel y increases downward.
    // Bottom of the plot area corresponds to area_.y + area_.h
    // Top of the plot area corresponds to area_.y
    double bottom = area_.y + area_.h;
    double top = area_.y;
    return {bottom, top};
}

void Axes::set_x_scale(std::shared_ptr<Scale> scale) {
    x_scale_ = std::move(scale);
}

void Axes::set_y_scale(std::shared_ptr<Scale> scale) {
    y_scale_ = std::move(scale);
}

const Scale& Axes::x_scale() const {
    if (!x_scale_) {
        throw InvalidArgument("X scale has not been set on Axes");
    }
    return *x_scale_;
}

const Scale& Axes::y_scale() const {
    if (!y_scale_) {
        throw InvalidArgument("Y scale has not been set on Axes");
    }
    return *y_scale_;
}

PointI Axes::to_pixel(double x, double y) const {
    const Scale& xs = x_scale();
    const Scale& ys = y_scale();
    int px = static_cast<int>(std::round(xs.map(x)));
    int py = static_cast<int>(std::round(ys.map(y)));
    return PointI{px, py};
}

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
                int ty = area_.y + area_.h + 7;
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
                int tx = area_.x - 7 - tw; // Right-aligned to the left of tick mark
                int ty = py - th / 2;      // Vertically centered
                draw_text(*canvas_, tx, ty, label, theme_.text_color, theme_.font_scale);
            }
        }
    }
}

} // namespace cppviz