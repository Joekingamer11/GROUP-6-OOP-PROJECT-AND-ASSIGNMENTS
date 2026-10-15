#include "cppviz/scale.hpp"
#include "cppviz/error.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace cppviz
{
  Scale::Scale(double domain_min, double domain_max, double range_min,
    double range_max)
    : domain_min_(std::min(domain_min, domain_max)),
      domain_max_(std::max(domain_min, domain_max)),
      range_min_(range_min),
      range_max_(range_max)
  {
    if (std::isnan(domain_min_) || std::isnan(domain_max_) ||
        std::isnan(range_min_) || std::isnan(range_max_))
    {
      throw InvalidArgument("Scale domain and range bounds cannot be NaN");
    }
    if (domain_min_ == domain_max_)
    {
      throw InvalidArgument("Scale domain_min and domain_max cannot be equal");
    }
  }

  double Scale::domain_min() const
  {
    return domain_min_;
  }

  double Scale::domain_max() const
  {
    return domain_max_;
  }

  std::string Scale::tick_label(double value) const
  {
    std::ostringstream oss;
    oss << value;
    return oss.str();
  }

  LinearScale::LinearScale(double domain_min, double domain_max,
      double range_min, double range_max)
      : Scale(domain_min, domain_max, range_min, range_max)
  {
  }

  double LinearScale::map(double value) const
  {
    const double t = (value - domain_min_) / (domain_max_ - domain_min_);
    return range_min_ + t * (range_max_ - range_min_);
  }

  double LinearScale::invert(double pixel) const
  {
    const double t = (pixel - range_min_) / (range_max_ - range_min_);
    return domain_min_ + t * (domain_max_ - domain_min_);
  }

  std::vector<double> LinearScale::tick_values() const
  {
    return {domain_min_, domain_max_};
  }

  std::string LinearScale::tick_label(double value) const
  {
    return Scale::tick_label(value);
  }
}


