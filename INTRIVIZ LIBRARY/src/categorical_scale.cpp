#include "cppviz/categorical_scale.hpp"
#include "cppviz/error.hpp"
#include <cmath>
#include <utility>

namespace cppviz {

CategoricalScale::CategoricalScale(std::vector<std::string> names, double range_min, double range_max)
    : Scale(-0.5, names.empty() ? 0.5 : static_cast<double>(names.size()) - 0.5, range_min, range_max),
      names_(std::move(names)) {
    require(!names_.empty(), "CategoricalScale requires at least one category name");
}

double CategoricalScale::map(double value) const {
    double domain_len = dmax_ - dmin_;
    if (domain_len == 0.0) return rmin_;
    double t = (value - dmin_) / domain_len;
    return rmin_ + t * (rmax_ - rmin_);
}

double CategoricalScale::invert(double pixel) const {
    double range_len = rmax_ - rmin_;
    if (range_len == 0.0) return dmin_;
    double t = (pixel - rmin_) / range_len;
    return dmin_ + t * (dmax_ - dmin_);
}

std::vector<double> CategoricalScale::tick_values() const {
    std::vector<double> ticks;
    ticks.reserve(names_.size());
    for (std::size_t i = 0; i < names_.size(); ++i) {
        ticks.push_back(static_cast<double>(i));
    }
    return ticks;
}

std::string CategoricalScale::tick_label(double value) const {
    int idx = static_cast<int>(std::round(value));
    if (idx >= 0 && static_cast<std::size_t>(idx) < names_.size()) {
        return names_[idx];
    }
    return "";
}

double CategoricalScale::band_width() const {
    if (names_.empty()) return 0.0;
    return std::abs(rmax_ - rmin_) / static_cast<double>(names_.size());
}

std::size_t CategoricalScale::category_count() const {
    return names_.size();
}

} // namespace cppviz   