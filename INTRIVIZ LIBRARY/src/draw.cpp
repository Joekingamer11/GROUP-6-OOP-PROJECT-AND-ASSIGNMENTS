#include "cppviz/draw.hpp"
#include "cppviz/error.hpp"

#include <cmath>
#include <cstdlib>
#include <string>

namespace cppviz {

void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color color, int thickness) {
    if (thickness <= 0) {
        return;
    }

    int dx = std::abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -std::abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    int offset_start = -thickness / 2;
    int offset_end = offset_start + thickness;

    while (true) {
        if (thickness == 1) {
            canvas.set_pixel(x0, y0, color);
        } else {
            // Stamp a filled square centered at (x0, y0) of width/height = thickness
            for (int oy = offset_start; oy < offset_end; ++oy) {
                for (int ox = offset_start; ox < offset_end; ++ox) {
                    canvas.set_pixel(x0 + ox, y0 + oy, color);
                }
            }
        }

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_rect(Canvas& canvas, int x, int y, int w, int h, Color color) {
    // Normalize negative dimensions
    if (w < 0) {
        x += w;
        w = -w;
    }
    if (h < 0) {
        y += h;
        h = -h;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    // Top and bottom horizontal edges
    for (int px = x; px < x + w; ++px) {
        canvas.set_pixel(px, y, color);
        if (h > 1) {
            canvas.set_pixel(px, y + h - 1, color);
        }
    }

    // Left and right vertical edges (skipping corners already rendered)
    for (int py = y + 1; py < y + h - 1; ++py) {
        canvas.set_pixel(x, py, color);
        if (w > 1) {
            canvas.set_pixel(x + w - 1, py, color);
        }
    }
}

void fill_rect(Canvas& canvas, int x, int y, int w, int h, Color color) {
    // Normalize negative dimensions
    if (w < 0) {
        x += w;
        w = -w;
    }
    if (h < 0) {
        y += h;
        h = -h;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int py = y; py < y + h; ++py) {
        for (int px = x; px < x + w; ++px) {
            canvas.set_pixel(px, py, color);
        }
    }
}

void draw_circle(Canvas& canvas, int cx, int cy, int radius, Color color) {
    if (radius < 0) {
        throw InvalidArgument("Circle radius cannot be negative, got " + std::to_string(radius));
    }
    if (radius == 0) {
        canvas.set_pixel(cx, cy, color);
        return;
    }

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    auto plot8 = [&](int px, int py) {
        canvas.set_pixel(cx + px, cy + py, color);
        canvas.set_pixel(cx - px, cy + py, color);
        canvas.set_pixel(cx + px, cy - py, color);
        canvas.set_pixel(cx - px, cy - py, color);
        canvas.set_pixel(cx + py, cy + px, color);
        canvas.set_pixel(cx - py, cy + px, color);
        canvas.set_pixel(cx + py, cy - px, color);
        canvas.set_pixel(cx - py, cy - px, color);
    };

    plot8(x, y);
    while (x < y) {
        ++x;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            --y;
            d += 2 * (x - y) + 1;
        }
        plot8(x, y);
    }
}

void fill_circle(Canvas& canvas, int cx, int cy, int radius, Color color) {
    if (radius < 0) {
        throw InvalidArgument("Circle radius cannot be negative, got " + std::to_string(radius));
    }
    if (radius == 0) {
        canvas.set_pixel(cx, cy, color);
        return;
    }

    int r2 = radius * radius;
    for (int dy = -radius; dy <= radius; ++dy) {
        int dy2 = dy * dy;
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy2 <= r2) {
                canvas.set_pixel(cx + dx, cy + dy, color);
            }
        }
    }
}

} // namespace cppviz