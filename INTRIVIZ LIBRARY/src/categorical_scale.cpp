/**
 * @file categorical_scale.cpp
 * @brief Implementation of discrete categorical scales for bar, point, and swarm plots.
 *
 * Maps discrete category indices to evenly divided bands or positions across
 * a pixel range, managing category names, tick placement, and band widths.
 */

#include "intriviz/categorical_scale.hpp"
#include "intriviz/error.hpp"
#include <cmath>
#include <utility>

namespace intriviz {

/**
 * @brief Constructs a CategoricalScale from an ordered list of category names and pixel bounds.
 * 
 * Defines domain boundaries as [-0.5, N - 0.5] where N is the number of categories,
 * placing category centers at integer positions 0, 1, ..., N-1.
 * 
 * @param names List of distinct category labels.
 * @param range_min Starting pixel coordinate of the scale.
 * @param range_max Ending pixel coordinate of the scale.
 * @throws InvalidArgument If names is empty.
 */
CategoricalScale::CategoricalScale(std::vector<std::string> names, double range_min, double range_max)
    : Scale(-0.5, names.empty() ? 0.5 : static_cast<double>(names.size()) - 0.5, range_min, range_max),
      names_(std::move(names)) {
    require(!names_.empty(), "CategoricalScale requires at least one category name");
}

/**
 * @brief Maps a continuous category coordinate (where integer values correspond to band centers) to pixel space.
 * 
 * @param value Continuous position in category index space.
 * @return Corresponding pixel coordinate.
 */
double CategoricalScale::map(double value) const {
    const double domain_len = domain_max_ - domain_min_;
    const double t = (value - domain_min_) / domain_len;
    return range_min_ + t * (range_max_ - range_min_);
}

/**
 * @brief Inverts a pixel coordinate to its continuous category domain coordinate.
 * 
 * @param pixel Pixel coordinate along the axis.
 * @return Continuous index in the category domain.
 */
double CategoricalScale::invert(double pixel) const {
    const double range_len = range_max_ - range_min_;
    if (range_len == 0.0) return domain_min_;
    const double t = (pixel - range_min_) / range_len;
    return domain_min_ + t * (domain_max_ - domain_min_);
}

/**
 * @brief Returns tick values positioned at the center of each category (0.0, 1.0, ..., N-1).
 * 
 * @return Vector of floating-point category indices.
 */
std::vector<double> CategoricalScale::tick_values() const {
    std::vector<double> ticks;
    ticks.reserve(names_.size());
    for (std::size_t i = 0; i < names_.size(); ++i) {
        ticks.push_back(static_cast<double>(i));
    }
    return ticks;
}

/**
 * @brief Looks up the category name for a given rounded index.
 * 
 * @param value The continuous or discrete category index.
 * @return The category name string, or an empty string if out of bounds.
 */
std::string CategoricalScale::tick_label(double value) const {
    int idx = static_cast<int>(std::round(value));
    if (idx >= 0 && static_cast<std::size_t>(idx) < names_.size()) {
        return names_[idx];
    }
    return "";
}

/**
 * @brief Computes the pixel width allocated to each individual category band.
 * 
 * @return Width in pixels for one category slot.
 */
double CategoricalScale::band_width() const {
    if (names_.empty()) return 0.0;
    return std::abs(range_max_ - range_min_) / static_cast<double>(names_.size());
}

/**
 * @brief Returns the total number of categories managed by this scale.
 * 
 * @return Number of category labels.
 */
std::size_t CategoricalScale::category_count() const {
    return names_.size();
}

} // namespace intriviz
