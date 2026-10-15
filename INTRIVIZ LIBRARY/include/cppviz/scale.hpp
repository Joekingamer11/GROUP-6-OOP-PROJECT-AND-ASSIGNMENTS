#pragma once 

#include <string>
#include <vector>

namespace cppviz
{
    class Scale {
    public:
    virtual ~Scale() = default;
    virtual double map(double value) const = 0;
    
    virtual double invert(double pixel) const = 0;
    virtual std::vector<double> tick_values() const = 0;
    virtual std::string tick_label(double value) const = 0;
    double domain_min() const;
    double domain_max() const;
protected:
Scale(double domain_min, double domain_max, double range_min, double range_max);
    double domain_min_;
    double domain_max_;
    double range_min_;
    double range_max_;
 
};
class LinearScale : public Scale {
    public:
    LinearScale(double domain_min, double domain_max, double range_min, double range_max);

    double map(double value) const override;
    double invert(double pixel) const override;
    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};
}


