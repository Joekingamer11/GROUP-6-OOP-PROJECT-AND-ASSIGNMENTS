#pragma once
#include <string>
#include <vector>
#include "cppviz/axes.hpp"

namespace cppviz {

/**
 * @brief Draws a boxed categorical color key inside the plot area.
 * 
 * Renders a legend box in the top-right corner of the plot area containing
 * a solid color swatch and category label for each entry.
 * 
 * @param ax The Axes target providing canvas and theme settings.
 * @param names Category text labels.
 * @param colors Corresponding color swatch for each category.
 * @param title Optional title text displayed at the top of the legend box.
 * 
 * @throws InvalidArgument If names and colors vectors have different lengths.
 * @note An empty category list leaves the canvas unchanged and draws nothing.
 */
void draw_legend(Axes& ax, 
                 const std::vector<std::string>& names,
                 const std::vector<Color>& colors, 
                 const std::string& title = "");

} 