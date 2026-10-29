#pragma once

#include <string>
#include "intriviz/canvas.hpp"

namespace intriviz {

// Width in pixels of the text, including the 1 pixel gap between characters.
// Empty strings give 0.
int text_width(const std::string& text, int scale = 1);

// Height in pixels of one line of text.
int text_height(int scale = 1);

// Draws the text with its top-left corner at (x, y).
// Characters we do not have a glyph for draw as a hollow box.
// Pixels that fall outside the canvas are simply ignored.
void draw_text(Canvas& canvas, int x, int y, const std::string& text, Color color, int scale = 1);

// Same as draw_text but rotated 90 degrees counter-clockwise.
// Used for the y-axis label, so the text reads bottom to top.
void draw_text_vertical(Canvas& canvas, int x, int y, const std::string& text, Color color, int scale = 1);

} // namespace intriviz