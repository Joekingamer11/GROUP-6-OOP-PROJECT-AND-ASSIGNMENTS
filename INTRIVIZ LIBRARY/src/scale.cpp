/**
 * @file scale.cpp
 * @brief Implementation of continuous mathematical scales (LinearScale, LogScale).
 *
 * Scales map data-space domain coordinates to pixel-space screen ranges,
 * and provide the inverse mappings as well as tick mark generation and formatting.
 */

#include "intriviz/scale.hpp"
#include "intriviz/error.hpp"
#include "intriviz/ticks.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace intriviz {

/**
 * @brief Constructs a base Scale mapping a numeric domain to a pixel range.
 * 
 * Automatically orders domain_min and domain_max so that domain_min_ <= domain_max_.
 * 
 * @param domain_min Minimum data-space value.
 * @param domain_max Maximum data-space value.
 * @param range_min Starting pixel-space coordinate.
 * @param range_max Ending pixel-space coordinate.
 * @throws InvalidArgument If any bound is NaN, or if domain_min equals domain_max.
 */
Scale::Scale(double domain_min, double domain_max, double range_min,
             double range_max)
    : domain_min_(std::min(domain_min, domain_max)),
      domain_max_(std::max(domain_min, domain_max)),
      range_min_(range_min),
      range_max_(range_max) {
    if (std::isnan(domain_min_) || std::isnan(domain_max_) ||
        std::isnan(range_min_) || std::isnan(range_max_)) {
        throw InvalidArgument("Scale domain and range bounds cannot be NaN");
    }
    if (domain_min_ == domain_max_) {
        throw InvalidArgument("Scale domain_min and domain_max cannot be equal");
    }
}

/**
 * @brief Retrieves the lower bound of the data domain.
 * 
 * @return The minimum domain value.
 */
double Scale::domain_min() const {
    return domain_min_;
}

/**
 * @brief Retrieves the upper bound of the data domain.
 * 
 * @return The maximum domain value.
 */
double Scale::domain_max() const {
    return domain_max_;
}

/**
 * @brief Constructs a LinearScale with linear interpolation between domain and range.
 * 
 * @param domain_min Lower data domain bound.
 * @param domain_max Upper data domain bound.
 * @param range_min Target pixel range start.
 * @param range_max Target pixel range end.
 */
LinearScale::LinearScale(double domain_min, double domain_max,
                         double range_min, double range_max)
    : Scale(domain_min, domain_max, range_min, range_max) {}

/**
 * @brief Maps a data value linearly into pixel space.
 * 
 * @param value The value in data coordinates.
 * @return The corresponding pixel coordinate.
 */
double LinearScale::map(double value) const {
    const double t = (value - domain_min_) / (domain_max_ - domain_min_);
    return range_min_ + t * (range_max_ - range_min_);
}

/**
 * @brief Inverts a pixel coordinate back to its original data domain value.
 * 
 * @param pixel The pixel coordinate along the axis.
 * @return The corresponding value in data domain space.
 */
double LinearScale::invert(double pixel) const {
    const double t = (pixel - range_min_) / (range_max_ - range_min_);
    return domain_min_ + t * (domain_max_ - domain_min_);
}

/**
 * @brief Generates default tick values representing the domain boundaries.
 * 
 * @return A vector containing {domain_min, domain_max}.
 */
std::vector<double> LinearScale::tick_values() const {
    return {domain_min_, domain_max_};
}

/**
 * @brief Converts a numeric tick value to its string representation.
 * 
 * @param value The tick value to format.
 * @return Formatted string representation of value.
 */
std::string LinearScale::tick_label(double value) const {
    std::ostringstream output;
    output << value;
    return output.str();
}

/**
 * @brief Constructs a logarithmic base-10 scale.
 * 
 * @param domain_min Lower domain bound (must be strictly positive).
 * @param domain_max Upper domain bound (must be strictly positive).
 * @param range_min Starting pixel coordinate.
 * @param range_max Ending pixel coordinate.
 * @throws InvalidArgument If domain_min or domain_max <= 0.
 */
LogScale::LogScale(double domain_min, double domain_max, double range_min,
                   double range_max)
    : Scale(domain_min, domain_max, range_min, range_max) {
    if (domain_min_ <= 0.0 || domain_max_ <= 0.0) {
        throw InvalidArgument("LogScale domain bounds must be positive");
    }
}

/**
 * @brief Maps a positive data value into pixel space using a logarithmic (log10) transformation.
 * 
 * @param value The data coordinate to map (must be positive).
 * @return The corresponding pixel coordinate.
 * @throws InvalidArgument If value <= 0.
 */
double LogScale::map(double value) const {
    if (value <= 0.0) {
        throw InvalidArgument("LogScale map value must be positive");
    }

    const double log_min = std::log10(domain_min_);
    const double log_max = std::log10(domain_max_);
    const double t = (std::log10(value) - log_min) / (log_max - log_min);
    return range_min_ + t * (range_max_ - range_min_);
}

/**
 * @brief Inverts a pixel coordinate back to data space through exponentiation (10^x).
 * 
 * @param pixel The pixel coordinate along the axis.
 * @return The mapped positive data value.
 */
double LogScale::invert(double pixel) const {
    const double t = (pixel - range_min_) / (range_max_ - range_min_);
    const double log_min = std::log10(domain_min_);
    const double log_max = std::log10(domain_max_);
    return std::pow(10.0, log_min + t * (log_max - log_min));
}

/**
 * @brief Computes power-of-ten tick marks spanning the logarithmic domain.
 * 
 * @return Vector of tick values (10^k) within [domain_min, domain_max].
 */
std::vector<double> LogScale::tick_values() const {
    return log_ticks(domain_min_, domain_max_);
}

/**
 * @brief Converts a logarithmic tick value to a string.
 * 
 * @param value The tick value to format.
 * @return String representation of the tick value.
 */
std::string LogScale::tick_label(double value) const {
    std::ostringstream output;
    output << value;
    return output.str();
}

} // namespace intriviz
