#include "cppviz/scale.hpp"
#include "cppviz/error.hpp"
#include "cppviz/ticks.hpp"

#include <cmath>
#include <sstream>

namespace cppviz
{
  Scale::Scale(double domain_min, double domain_max, double range_min, 
    double range_max)
    : dmin_(domain_max), dmax_(domain_max), rmin_(range_min), 
    rmax_(range_max){
        if (std::isnan(dmin_) || std::isnan(dmax_) || std::isnan(rmin_)
         || std::isnan(rmax_))
        {
            throw InvalidArgument("Scale domain and range bounds cannot be NaN");
        }
        if (dmin_ == dmax_){
            throw InvalidArgument("Scale domain_min and domain_max cannot be equal");
        }
    }
double Scale::domain_min() const{
    return dmin_;
}
double Scale::domain_max() const(){
    return dmax_;
}
std::string Scale::tick_label(double value) const{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
LinearScale::LinearScale(double domain_min, double domain_max, 
        double range_min, double range_max)
        : Scale(domain_min, domain_max, range_min, range_max)
        {}
double LinearScale::map(double value) const{
        double t = (value - dmin_) / (dmax_ - dmin_);
        return rmin_ + t * (rmax_ - rmin_);
}
double LinearScale::invert(double pixel) const{
    double t = (pixel - rmin_) / (rmax_ - rmin_);
        return dmin_ + t * (dmax_ - dmin_);
}
std::vector<double> LinearScale::tick_values()
const{
    
    return {dmin_, dmax_};
}
LogScale::LogScale(double domain_min, double domain_max, 
        double range_min, double range_max)
        : Scale(domain_min, domain_max, range_min, range_max)
        {
            if (dmin_ <= 0 || dmax_ <= 0){
                throw InvalidArgument("LogScale domain bounds must be positive");
            }
        }
double LogScale::map(double value) const{
        if (value <= 0){
            throw InvalidArgument("LogScale map value must be positive");
        }
        double log_min = std::log10(dmin_);
        double log_max = std::log10(dmax_);
        double t = (std::log10(value) - log_min) / (log_max - log_min);
        return rmin_ + t * (rmax_ - rmin_);
} 
double LogScale::invert(double pixel) const{
       double t = (pixel - rmin_) / (rmax_ - rmin_);
        double log_dmin = std::log10(dmin_);
        double log_dmax = std::log10(dmax_);
        return std::pow(10, log_dmin + t * (log_dmax - log_dmin));
}
std::vector<double> LogScale::tick_values() const{
        return log_ticks(dmin_, dmax_);
}   
}


