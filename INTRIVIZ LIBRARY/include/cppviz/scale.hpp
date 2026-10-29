#pragma once 

#include <string>
#include <vector>

namespace cppviz

//Base class for mapping continous or discrete data values to pixel cordinates
    /*It defines the core contrct for cordinate transformation, range inversion,, tick selection
and tick label formatting across all concrete scale types*/

{
    class Scale {
    public:

    virtual ~Scale() = default;
    
    //This maps a scalar data coordinate to a canvas position
    virtual double map(double value) const = 0;

    //Inverts a pixel coordinate back to its original data space value
    virtual double invert(double pixel) const = 0;

    //Returns a list of tick mark positions in data coordinates
    virtual std::vector<double> tick_values() const = 0;

    //Formats a tick data value into a human-readable display string
    virtual std::string tick_label(double value) const = 0;

    //Min bound of the data domain
    double domain_min() const;

    //max bound of the data domain
    double domain_max() const;

protected:

//A protected base constructor validating domain and range inputs
Scale(double domain_min, double domain_max, double range_min, double range_max);
    double domain_min_;
    double domain_max_;
    double range_min_;
    double range_max_;
 
};

//Linearly scales continous data values to pixel coordinates
class LinearScale : public Scale {
    public:

    //This constructs a linear scale mapping
    LinearScale(double domain_min, double domain_max, double range_min, double range_max);

    double map(double value) const override;
    double invert(double pixel) const override;

    //This generates round, human friendly tick positions
    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};

//To logarithmically scale strictky positive data values to pixel coordintes
class LogScale : public Scale {
    public:

    /*Constructs a base-10 logarithmic scale
         throws InvalidArgument if any input is Nan*/
    LogScale(double domain_min, double domain_max, double range_min, double range_max);

    /* Maps positive data cordinate to pixels using std::log10*/
    double map(double value) const override;

    double invert(double pixel) const override;
    
    
    std::vector<double> tick_values() const override;
    std::string tick_label(double value) const override;
};
}


