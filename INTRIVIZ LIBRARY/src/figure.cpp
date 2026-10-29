#include "cppviz/figure.hpp"
#include "cppviz/error.hpp"
#include "cppviz/image_export.hpp"
#include <utility>

namespace cppviz {

namespace {

// Helper to validate and compute the Axes rectangle area from canvas dimensions and margins
Rect compute_axes_area(int width, int height, const Margins& margins) {
    int w = width - margins.left - margins.right;
    int h = height - margins.top - margins.bottom;
    require(w >= 10 && h >= 10, "Figure margins leave less than 10x10 pixels for plot area");
    return Rect{margins.left, margins.top, w, h};
}

} // anonymous namespace

Figure::Figure(int width, int height, const Theme& theme)
    : canvas_(std::make_unique<Canvas>(width, height, theme.background)),
      axes_(*canvas_, compute_axes_area(width, height, Margins{}), theme),
      margins_() {}

Axes& Figure::axes() {
    return axes_;
}

const Axes& Figure::axes() const {
    return axes_;
}

Canvas& Figure::canvas() {
    return *canvas_;
}

const Canvas& Figure::canvas() const {
    return *canvas_;
}

const Theme& Figure::theme() const {
    return axes_.theme();
}

void Figure::set_margins(const Margins& margins) {
    Rect new_area = compute_axes_area(canvas_->width(), canvas_->height(), margins);
    margins_ = margins;
    axes_.set_area(new_area);
}

const Margins& Figure::margins() const {
    return margins_;
}

void Figure::save(const std::string& path) const {
    save_image(*canvas_, path);
}

} // namespace cppviz