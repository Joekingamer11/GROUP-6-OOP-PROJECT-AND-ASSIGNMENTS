#pragma once

#include <memory>
#include <utility>
#include "intriviz/canvas.hpp"
#include "intriviz/scale.hpp"
#include "intriviz/theme.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

class Axes {
public:
    Axes(Canvas& canvas, Rect area, const Theme& theme);

    const Rect& area() const;
    void set_area(const Rect& area);

    Canvas& canvas();
    const Theme& theme() const;

    // Returns pixel ranges: x_pixel_range = {left, right}, y_pixel_range = {bottom, top}
    // Note: y_pixel_range is reversed so that larger data values map to higher pixels (smaller y).
    std::pair<double, double> x_pixel_range() const;
    std::pair<double, double> y_pixel_range() const;

    void set_x_scale(std::shared_ptr<Scale> scale);
    void set_y_scale(std::shared_ptr<Scale> scale);

    const Scale& x_scale() const; // Throws InvalidArgument if not set
    const Scale& y_scale() const; // Throws InvalidArgument if not set

    // Maps a data coordinate (x, y) to a pixel position on the canvas using the current scales
    PointI to_pixel(double x, double y) const;

    // Draws plot background, grid lines (if theme.show_grid is true), axis lines, tick marks, and tick labels
    void draw_frame();

private:
    Canvas* canvas_; // Pointer to Canvas so Axes remains valid when moved or copied
    Rect area_;
    Theme theme_;
    std::shared_ptr<Scale> x_scale_;
    std::shared_ptr<Scale> y_scale_;
};

} // namespace intriviz