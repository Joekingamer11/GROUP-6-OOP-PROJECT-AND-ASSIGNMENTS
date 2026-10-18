#include "cppviz/ticks.hpp"
#include "cppviz/error.hpp"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

namespace cppviz {

std::vector<double> nice_ticks(double min, double max, int target_count) {
    if (std::isnan(min) || std::isnan(max)) {
        throw InvalidArgument("nice_ticks arguments cannot be NaN");
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

    const double range = max - min;
    const double raw_step = range / (target_count - 1);
    const double magnitude = std::pow(10.0, std::floor(std::log10(raw_step)));
    const double fraction = raw_step / magnitude;

    double nice_fraction;
    if (fraction < 1.5) {
        nice_fraction = 1.0;
    } else if (fraction < 2.25) {
        nice_fraction = 2.0;
    } else if (fraction < 3.75) {
        nice_fraction = 2.5;
    } else if (fraction < 7.5) {
        nice_fraction = 5.0;
    } else {
        nice_fraction = 10.0;
    }

    const double step = nice_fraction * magnitude;
    double start = std::floor(min / step) * step;
    const double end = std::ceil(max / step) * step;
    if (std::abs(start) < 1e-12 * step) {
        start = 0.0;
    }

    std::vector<double> ticks;
    for (int i = 0;; ++i) {
        double value = start + i * step;
        if (std::abs(value) < 1e-12 * step) {
            value = 0.0;
        }
        if (value > end + 1e-9 * step) {
            break;
        }
        ticks.push_back(value);
    }
    return ticks;
}

std::vector<double> log_ticks(double min, double max) {
    if (std::isnan(min) || std::isnan(max) || min <= 0.0 || max <= 0.0) {
        throw InvalidArgument("log_ticks domain bounds must be positive");
    }
    if (min > max) {
        throw InvalidArgument("min cannot be greater than max in log_ticks");
    }

    const int start_exp = static_cast<int>(std::ceil(std::log10(min) - 1e-9));
    const int end_exp = static_cast<int>(std::floor(std::log10(max) + 1e-9));

    std::vector<double> ticks;
    for (int exponent = start_exp; exponent <= end_exp; ++exponent) {
        ticks.push_back(std::pow(10.0, exponent));
    }
    return ticks;
}

std::string format_tick(double value) {
    if (std::isnan(value)) {
        return "NaN";
    }
    if (std::isinf(value)) {
        return value > 0.0 ? "Inf" : "-Inf";
    }
    if (value == 0.0) {
        return "0";
    }

    std::ostringstream output;
    const double abs_value = std::abs(value);
    if (abs_value < 1e-4 || abs_value >= 1e6) {
        output << std::scientific << std::setprecision(4) << value;
    } else {
        output << std::fixed << std::setprecision(6) << value;
    }

    std::string result = output.str();
    const std::size_t exponent_position = result.find_first_of("eE");
    const std::size_t mantissa_end =
        exponent_position == std::string::npos ? result.size() : exponent_position;
    const std::size_t decimal_position = result.find('.');
    if (decimal_position != std::string::npos && decimal_position < mantissa_end) {
        std::size_t trim_position = mantissa_end;
        while (trim_position > decimal_position + 1 && result[trim_position - 1] == '0') {
            --trim_position;
        }
        if (trim_position == decimal_position + 1) {
            --trim_position;
        }
        result.erase(trim_position, mantissa_end - trim_position);
    }
    return result;
}

std::pair<double, double> padded_range(const std::vector<double>& values,
                                      double padding) {
    if (values.empty()) {
        throw DataError("padded_range requires a non-empty vector of values");
    }

    double min_value = std::numeric_limits<double>::infinity();
    double max_value = -std::numeric_limits<double>::infinity();
    bool has_valid_value = false;

    for (double value : values) {
        if (!std::isnan(value)) {
            has_valid_value = true;
            if (value < min_value) min_value = value;
            if (value > max_value) max_value = value;
        }
    }
    if (!has_valid_value) {
        throw DataError("padded_range vector contains only NaN values");
    }

    if (min_value == max_value) {
        return {min_value - 0.5, max_value + 0.5};
    }

    const double span = max_value - min_value;
    return {min_value - span * padding, max_value + span * padding};
}

} // namespace cppviz
