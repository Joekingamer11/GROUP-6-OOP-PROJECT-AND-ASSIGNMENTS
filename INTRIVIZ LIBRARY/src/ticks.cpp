#include "cppviz/ticks.hpp"
#include "cppviz/error.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;
namespace cppviz {

    std::vector<double> nice_ticks(double min, double max, 
        int target_count) {
        if (std::isnan(min) || std::isnan(max)) {
            throw InvalidArgument("nice_ticks arguments cannot 
                be NaN");
        }
    
        if (min > max) {
            throw InvalidArgument("min must be less than or equal to max in nice_ticks");
        }
        if (target_count < 2) {
            throw InvalidArgument("target_count must be at least 2");
        }

       if (min == max) {
            return {min};
        }
    }
        double range = max - min;
        double raw_step = range / (target_count - 1);
        double magnitude = std::pow(10, std::floor(std::log10(raw_step)));
        double fraction = raw_step / magnitude;

        double nice_fraction;
        if (fraction <= 1.0) {
            nice_fraction = 1.0;
        } else if (fraction <= 2.0) {
            nice_fraction = 2.0;
        } else if (fraction <= 2.5) {
            nice_fraction = 2.5;
        } else if (fraction <= 5.0) {
            nice_fraction = 5.0;
        } else {
            nice_fraction = 10.0;
        }

        double step = nice_fraction * magnitude;
        double start = std::ceil(min / step) * step;

        if(std::abs(start) < 1e-12 * step) {
            start = 0.0; // Avoid negative zero
        }
        
        std::vector<double> ticks;
        for (int i = 0; ; i++) {
            double val = start + i * step;
            if (std::abs(val) < 1e-12 * step) {
                val = 0.0; // Avoid negative zero
                break;
            }
           if (val > max + 1e-9 * step) { // Add a small epsilon to account for floating-point precision issues
                break;
            }
            ticks.push_back(val);
        }
        return ticks;
    }

std::vector<double> log_ticks(double min, double max) {
        if (std::isnan(min) || std::isnan(max) || min <= 0 || max <= 0) {
            throw InvalidArgument("log_ticks domain bounds must be positive");

        }
        if (min > max ) {
            throw InvalidArgument("min cannot be greater than max in log_ticks");
        
        }

        int start_exp = static_cast<int>(std::floor(std::log10(min) - 1e-9));
        int end_exp = static_cast<int>(std::ceil(std::log10(max) 
        + 1e-9));

        std::vector<double> ticks;
        for (int exp = start_exp; exp <= end_exp; ++exp) {
            ticks.push_back(std::pow(10, exp));
        }
        return ticks;
    }

    std::string format_tick(double value) {
        if(std::isnan(value)) return "NaN";
        if (std::isinf(value)) return (value > 0) ? "-Inf" : "Inf";
        if (value == 0.0) return "0";


        std::ostringstream oss;
        double abs_val = std::abs(value);

        if ((abs_val > 0 && abs_val < 1e-4) || abs_val >= 1e6)
         {
            oss << std::scientific << std::setprecision(4) << value;
            std::string result = oss.str(); 
            // Remove trailing zeros and the decimal point if necessary
            auto e_pos = str.find_firstof("eE");
            if (e_pos != std::string::npos) {
                std::string mantissa = str.substr(0, e_pos);
                std::string exponent = str.substr(e_pos);
               if (mantissa.find('.') != std::string::npos) {
                    while (!mantissa.empty() && mantissa.back() == '0') {
                        mantissa.pop_back();
                    }
                    if (!mantissa.empty() && mantissa.back() == '.') {
                        mantissa.pop_back();
                    }
                    return mantissa + exponent;
            }

            return str;
        } else {
            oss << std::fixed << std::setprecision(6) << value;
            std::string str = oss.str();
            // Remove trailing zeros and the decimal point if necessary
            if (str.find('.') != std::string::npos) {
                while (!str.empty() && str.back() == '0') {
                    str.pop_back();
                }
                if (!str.empty() && str.back() == '.') {
                    str.pop_back();
                }
            }
            return str;
        }
    }

    std::pair<double, double> padded_range(const
         std::vector<double>& values, double padding) {
        if (values.empty()){
            throw DataError("padded_range requires a non-empty vector of values");
        }
       }
    
       double min_val = std::numeric_limits<double>::infinity();
         double max_val = -std::numeric_limits<double>::infinity();
    
         bool has_valid_value = false;

        for (double v : values) {
            if (!std::isnan(v)) {
                has_valid_value = true;
                if (v < min_val) min_val = v;
                if (v > max_val) max_val = v;
            }
    }
       if (!has_valid) {
        throw DataError("padded_range vector contains only NaN values");
       } 

       if (min_val == max_val) {
            // If all values are the same, we can return a small range around that value
            
            return {min_val -0.5, max_val + 0.5};
        }

       double span = max_val - min_val;
       return {min_val - span * padding, max_val + span * padding};
    }
}