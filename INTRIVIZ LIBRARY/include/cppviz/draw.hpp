#pragma once

#include "cppviz/canvas.hpp"

namespace cppviz {

/**
 * @brief Draws a straight line between two points using Bresenham's algorithm.
 * 
 * Stepping occurs along the major driving axis with an integer error term.
 * Supports an optional thickness parameter (default: 1).
 * Out-of-bounds coordinates are clipped silently by the underlying Canvas.
 * 
 * @param canvas Target Canvas to draw on.
 * @param x0 Starting X coordinate.
 * @param y0 Starting Y coordinate.
 * @param x1 Ending X coordinate.
 * @param y1 Ending Y coordinate.
 * @param color Stroke Color struct.
 * @param thickness Line stroke width in pixels (default: 1).
 */
void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color color, int thickness = 1);

/**
 * @brief Draws the outline of a rectangle.
 * 
 * Negative widths or heights are automatically normalized.
 * Boundary clipping is handled safely by the Canvas.
 * 
 * @param canvas Target Canvas to draw on.
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param w Rectangle width in pixels.
 * @param h Rectangle height in pixels.
 * @param color Stroke Color struct.
 */
void draw_rect(Canvas& canvas, int x, int y, int w, int h, Color color);

/**
 * @brief Draws a solid filled rectangle.
 * 
 * Negative widths or heights are automatically normalized.
 * Boundary clipping is handled safely by the Canvas.
 * 
 * @param canvas Target Canvas to draw on.
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param w Rectangle width in pixels.
 * @param h Rectangle height in pixels.
 * @param color Fill Color struct.
 */
void fill_rect(Canvas& canvas, int x, int y, int w, int h, Color color);

/**
 * @brief Draws the 1-pixel outline of a circle using the midpoint circle algorithm.
 * 
 * @param canvas Target Canvas to draw on.
 * @param cx Circle center X coordinate.
 * @param cy Circle center Y coordinate.
 * @param radius Circle radius in pixels (must be non-negative).
 * @param color Stroke Color struct.
 * @throws InvalidArgument If radius < 0.
 */
void draw_circle(Canvas& canvas, int cx, int cy, int radius, Color color);

/**
 * @brief Draws a solid filled circle.
 * 
 * Fills all pixels where (x - cx)^2 + (y - cy)^2 <= radius^2.
 * 
 * @param canvas Target Canvas to draw on.
 * @param cx Circle center X coordinate.
 * @param cy Circle center Y coordinate.
 * @param radius Circle radius in pixels (must be non-negative).
 * @param color Fill Color struct.
 * @throws InvalidArgument If radius < 0.
 */
void fill_circle(Canvas& canvas, int cx, int cy, int radius, Color color);

} // namespace cppviz