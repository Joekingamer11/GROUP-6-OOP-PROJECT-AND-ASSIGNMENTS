#pragma once

#include <string>
#include "intriviz/axes.hpp"
#include "intriviz/types.hpp"

namespace intriviz {
/// Renders a text label at specified data coordinates (x, y).
/// @param ax Reference to target Axes.
/// @param x X coordinate in data units.
/// @param y Y coordinate in data units.
/// @param text Label content to render.
/// @throws InvalidArgument If axes scales have not been configured.
/// Annotates text at specific data coordinates (x, y).
/// Throws InvalidArgument if axes scales are not set.
void annotate_text(Axes& ax, double x, double y, const std::string& text);

/// Draws a horizontal reference line spanning the plot area at data coordinate y.
/// @param ax Reference to target Axes.
/// @param y Y coordinate in data units.
/// @param color Line color (default: dark gray).
/// @param thickness Line width in pixels.
/// @throws InvalidArgument If axes scales have not been configured.
/// Clipped to the plot area boundaries.
void annotate_hline(Axes& ax, double y, Color color = Color{38, 38, 38}, int thickness = 1);

/// Draws a vertical reference line spanning the plot area at data coordinate x.
/// @param ax Reference to target Axes.
/// @param x X coordinate in data units.
/// @param color Line color (default: dark gray).
/// @param thickness Line width in pixels.
/// @throws InvalidArgument If axes scales have not been configured.
/// Clipped to the plot area boundaries.
void annotate_vline(Axes& ax, double x, Color color = Color{38, 38, 38}, int thickness = 1);

/// Renders an arrow pointing from (x_from, y_from) to (x_to, y_to) in data coordinates.
/// @param ax Reference to target Axes.
/// @param x_from Tail X coordinate in data units.
/// @param y_from Tail Y coordinate in data units.
/// @param x_to Head X coordinate in data units.
/// @param y_to Head Y coordinate in data units.
/// @param text Optional text label displayed near the arrow tail.
/// @throws InvalidArgument If axes scales have not been configured.

/// Arrowhead wings are calculated using std::atan2.
void annotate_arrow(Axes& ax, double x_from, double y_from, double x_to, double y_to,
                    const std::string& text = "");

} 