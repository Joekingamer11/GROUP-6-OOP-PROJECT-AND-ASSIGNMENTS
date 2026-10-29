#pragma once

#include <string>
#include <utility>
#include <vector>

namespace intriviz {

    //to generate round, evenly spaced "nice" ticks for a given range
    
    /*Target: desired approximate no. of tick marks(6)
    Returns: vector of nicely spaced tick values
    Throws InvalidArgument when min > max, target_count < 2, and bounds are NaN*/
    std::vector<double> nice_ticks(double min, double max, 
        int target_count = 6);

    //to generate base-10 logarithmic tick marks (powers of 10) within [min, max]
    //it gives an invalid argument if min<= 0, max <= 0, min> max or bound are NaN
    std::vector<double> log_ticks(double min, double max);

    
    //it formarts a floating point tick value as a string without trailing zeros
    std::string format_tick(double value);

        
    //it calculates a padded range for a given set of values, adding a specified padding percentage to the min and max values
    std::pair<double, double> padded_range(const std::vector<double>& values, 
        double padding = 0.05);

}