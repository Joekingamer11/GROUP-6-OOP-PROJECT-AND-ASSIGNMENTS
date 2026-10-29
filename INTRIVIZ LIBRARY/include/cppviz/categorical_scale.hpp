#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "cppviz/scale.hpp"

namespace cppviz {

class CategoricalScale : public Scale {
public:
    // Construct with category names and pixel range. Throws InvalidArgument if names is empty.
    CategoricalScale(std::vector<std::string> names, double range_min, double range_max);

    double map(double value) const override;             // category index (0, 1, ...) -> pixel center
    double invert(double pixel) const override;           // pixel location -> category index
    std::vector<double> tick_values() const override;    // returns {0, 1, ..., n-1}
    std::string tick_label(double value) const override; // looks up category name by rounded index

    double band_width() const;                           // pixel width allocated per category
    std::size_t category_count() const;

private:
    std::vector<std::string> names_;
};

} // namespace cppviz