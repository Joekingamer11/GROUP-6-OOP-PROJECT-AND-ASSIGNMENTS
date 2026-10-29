#pragma once

#include "intriviz/axes.hpp"
#include "intriviz/colormap.hpp"

namespace intriviz {

/**
 * @brief Draws a vertical colorbar gradient strip with tick labels beside a plot area.
 * @param ax Target Axes containing the canvas and plot area dimensions.
 * @param colormap Colormap instance used to sample gradient colors.
 * @param vmin Minimum data value (mapped to bottom of colorbar).
 * @param vmax Maximum data value (mapped to top of colorbar).
 * @throws InvalidArgument if vmin >= vmax.
 */
void draw_colorbar(Axes& ax, const Colormap& colormap, double vmin, double vmax);

} // namespace intriviz