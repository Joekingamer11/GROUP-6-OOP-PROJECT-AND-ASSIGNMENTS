#pragma once

#include <string>
#include "cppviz/axes.hpp"
#include "cppviz/types.hpp"

namespace cppviz {

/// Annotates text at specific data coordinates (x, y).
/// Throws InvalidArgument if axes scales are not set.
void annotate_text(Axes& ax, double x, double y, const std::string& text);

/// Draws a horizontal reference line across the plot area at data coordinate y.
/// Clipped to the plot area boundaries.
void annotate_hline(Axes& ax, double y, Color color = Color{38, 38, 38}, int thickness = 1);

/// Draws a vertical reference line across the plot area at data coordinate x.
/// Clipped to the plot area boundaries.
void annotate_vline(Axes& ax, double x, Color color = Color{38, 38, 38}, int thickness = 1);

/// Draws an arrow from (x_from, y_from) to (x_to, y_to) in data coordinates with optional text at the tail.
/// Arrowhead wings are calculated using std::atan2.
void annotate_arrow(Axes& ax, double x_from, double y_from, double x_to, double y_to,
                    const std::string& text = "");

} 