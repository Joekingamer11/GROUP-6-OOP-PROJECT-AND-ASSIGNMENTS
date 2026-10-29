#include "intriviz/categorical_scale.hpp"
#include "intriviz/error.hpp"
#include <cmath>
#include <utility>

namespace intriviz {

CategoricalScale::CategoricalScale(std::vector<std::string> names, double range_min, double range_max)
    : Scale(-0.5, names.empty() ? 0.5 : static_cast<double>(names.size()) - 0.5, range_min, range_max),
      names_(std::move(names)) {
    require(!names_.empty(), "CategoricalScale requires at least one category name");
}

double CategoricalScale::map(double value) const {
    const double domain_len = domain_max_ - domain_min_;
    const double t = (value - domain_min_) / domain_len;
    return range_min_ + t * (range_max_ - range_min_);
}

double CategoricalScale::invert(double pixel) const {
    const double range_len = range_max_ - range_min_;
    if (range_len == 0.0) return domain_min_;
    const double t = (pixel - range_min_) / range_len;
    return domain_min_ + t * (domain_max_ - domain_min_);
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
    return std::abs(range_max_ - range_min_) / static_cast<double>(names_.size());
}

std::size_t CategoricalScale::category_count() const {
    return names_.size();
}

} // namespace intriviz   