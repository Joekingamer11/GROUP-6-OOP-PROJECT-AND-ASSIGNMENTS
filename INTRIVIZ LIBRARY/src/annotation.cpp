/**
 * @file annotation.cpp
 * @brief Implementation of plot annotation tools (text, horizontal/vertical lines, arrows).
 *
 * Provides data-coordinate based annotations that overlay on an Axes plot area,
 * including text callouts, reference lines (hline/vline), and directional arrows with optional labels.
 */

#include "intriviz/annotation.hpp"
#include "intriviz/draw.hpp"
#include "intriviz/font.hpp"
#include "intriviz/error.hpp"

#include <cmath>
#include <algorithm>

namespace intriviz {

namespace {

/**
 * @brief Helper to verify that both X and Y scales have been assigned to the Axes.
 * 
 * @param ax Axes instance to check.
 * @throws InvalidArgument If either scale has not been initialized.
 */
void check_scales_set(const Axes& ax) {
    (void)ax.x_scale();
    (void)ax.y_scale();
}

} // anonymous namespace

/**
 * @brief Overlays a text string at specified data coordinates (x, y).
 * 
 * Maps data point (x, y) into pixel coordinates and renders text using
 * the axes theme font settings.
 * 
 * @param ax Target Axes object.
 * @param x Horizontal coordinate in data space.
 * @param y Vertical coordinate in data space.
 * @param text Annotation text to render.
 */
void annotate_text(Axes& ax, double x, double y, const std::string& text) {
    check_scales_set(ax);
    if (text.empty()) {
        return;
    }

    PointI p = ax.to_pixel(x, y);

    // Render text starting at pixel position corresponding to data (x, y)
    draw_text(ax.canvas(), p.x, p.y, text, ax.theme().text_color, ax.theme().font_scale);
}

/**
 * @brief Draws a horizontal reference line spanning the full width of the plot area at data height y.
 * 
 * Clips drawing if the resulting pixel coordinate falls outside the vertical plot bounds.
 * 
 * @param ax Target Axes object.
 * @param y Vertical coordinate in data space.
 * @param color Stroke color of the line.
 * @param thickness Line stroke width in pixels.
 */
void annotate_hline(Axes& ax, double y, Color color, int thickness) {
    check_scales_set(ax);

    // Map y data coordinate to y pixel position
    PointI p = ax.to_pixel(0.0, y);
    const Rect& area = ax.area();

    // Clip: do not draw if line y-pixel falls outside the plot area height
    if (p.y < area.y || p.y >= area.y + area.h) {
        return;
    }

    // Draw horizontal line across the full width of the plot area
    int x0 = area.x;
    int x1 = area.x + area.w - 1;
    draw_line(ax.canvas(), x0, p.y, x1, p.y, color, thickness);
}

/**
 * @brief Draws a vertical reference line spanning the full height of the plot area at data position x.
 * 
 * Clips drawing if the resulting pixel coordinate falls outside the horizontal plot bounds.
 * 
 * @param ax Target Axes object.
 * @param x Horizontal coordinate in data space.
 * @param color Stroke color of the line.
 * @param thickness Line stroke width in pixels.
 */
void annotate_vline(Axes& ax, double x, Color color, int thickness) {
    check_scales_set(ax);

    // Map x data coordinate to x pixel position
    PointI p = ax.to_pixel(x, 0.0);
    const Rect& area = ax.area();

    // Clip: do not draw if line x-pixel falls outside the plot area width
    if (p.x < area.x || p.x >= area.x + area.w) {
        return;
    }

    // Draw vertical line across the full height of the plot area
    int y0 = area.y;
    int y1 = area.y + area.h - 1;
    draw_line(ax.canvas(), p.x, y0, p.x, y1, color, thickness);
}

/**
 * @brief Draws a directional arrow between two data points with an optional text label at the arrow tail.
 * 
 * Computes vector angles to orient the arrow wings at +/- 150 degrees from the shaft direction.
 * 
 * @param ax Target Axes object.
 * @param x_from Starting point X data coordinate.
 * @param y_from Starting point Y data coordinate.
 * @param x_to Target point X data coordinate (tip of arrow).
 * @param y_to Target point Y data coordinate (tip of arrow).
 * @param text Optional text note displayed near the arrow origin.
 */
void annotate_arrow(Axes& ax, double x_from, double y_from, double x_to, double y_to,
                    const std::string& text) {
    check_scales_set(ax);

    PointI p_from = ax.to_pixel(x_from, y_from);
    PointI p_to = ax.to_pixel(x_to, y_to);

    // Draw main arrow shaft
    draw_line(ax.canvas(), p_from.x, p_from.y, p_to.x, p_to.y, ax.theme().text_color, ax.theme().line_thickness);

    // Calculate direction angle of shaft using std::atan2
    double dx = static_cast<double>(p_to.x - p_from.x);
    double dy = static_cast<double>(p_to.y - p_from.y);
    double angle = std::atan2(dy, dx);

    // Compute arrowhead wing points at +/- 150 degrees (5*pi/6) from shaft direction
    const double head_length = 10.0;
    const double pi = std::acos(-1.0);
    const double angle_offset = 150.0 * (pi / 180.0);

    double angle1 = angle + angle_offset;
    double angle2 = angle - angle_offset;

    int head1_x = static_cast<int>(p_to.x + head_length * std::cos(angle1));
    int head1_y = static_cast<int>(p_to.y + head_length * std::sin(angle1));

    int head2_x = static_cast<int>(p_to.x + head_length * std::cos(angle2));
    int head2_y = static_cast<int>(p_to.y + head_length * std::sin(angle2));

    // Draw arrowhead wings
    draw_line(ax.canvas(), p_to.x, p_to.y, head1_x, head1_y, ax.theme().text_color, ax.theme().line_thickness);
    draw_line(ax.canvas(), p_to.x, p_to.y, head2_x, head2_y, ax.theme().text_color, ax.theme().line_thickness);

    // Render optional text note at the arrow tail
    if (!text.empty()) {
        draw_text(ax.canvas(), p_from.x, p_from.y - 12, text, ax.theme().text_color, ax.theme().font_scale);
    }
}

} // namespace intriviz
