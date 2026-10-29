#pragma once

#include <string>
#include "intriviz/axes.hpp"

namespace intriviz {

// Draws a title centred horizontally above the plot area
void draw_title(Axes& ax, const std::string& title);

// Draws an x-axis label centred horizontally below the tick labels
void draw_xlabel(Axes& ax, const std::string& label);

// Draws a y-axis label (rotated vertically) centred to the left of the tick labels
void draw_ylabel(Axes& ax, const std::string& label);

} // namespace intriviz