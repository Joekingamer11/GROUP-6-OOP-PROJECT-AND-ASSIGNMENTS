# Week 1

## Files added

### `include/cppviz/image_export.hpp`

This file declares three functions that save a canvas as an image: `write_ppm`, `write_bmp` and `save_image`. The first two write one specific format each. `save_image` picks the format from the file extension and throws `InvalidArgument` if it is anything other than `.ppm` or `.bmp`. All three take a `const Canvas&` so they do not change the picture they are given.

### `src/image_export.cpp`

This file implements the three functions.

`write_ppm` writes a P6 PPM file. It puts a small text header first (the magic number `P6`, the width and height, then `255`), then the raw RGB bytes of every pixel going row by row. PPM is the simplest image format, so it is the easiest one to write and test.

`write_bmp` writes a 24-bit BMP file. It writes a 14-byte file header and a 40-byte info header, then the pixels. Every number in the header is written little-endian, one byte at a time, using two small helpers called `write_u16` and `write_u32`. Those helpers live inside an anonymous namespace so they are not part of the public API. The pixels follow three rules the BMP format needs: rows go bottom to top, each pixel is written in blue-green-red order instead of red-green-blue, and each row is padded with zero bytes until its length is a multiple of four.

`save_image` checks the last four characters of the path. If they are `.ppm` it calls `write_ppm`, if they are `.bmp` it calls `write_bmp`, otherwise it throws `InvalidArgument` with the path in the message. The two writers throw `IoError` if the file cannot be opened for writing.

### `tests/test_image_export.cpp`

This file tests the three functions with normal input, edge cases, and invalid input.

It makes a small 3x2 canvas with two coloured pixels, writes it with `write_ppm`, opens the file and checks it starts with the P6 magic number. Then it writes the same canvas with `write_bmp` and checks the first two bytes are `B` and `M`. Both files are deleted at the end so nothing is left behind. Finally it checks that `save_image` throws `InvalidArgument` when the extension is something like `.gif`.

The tests use the shared macros from `tests/mini_test.hpp`: `CHECK`, `CHECK_EQ` and `CHECK_THROWS`. They end with `return finish();` so `ctest` sees a failure if any check fails.

## How the files work together

The header is the public interface other parts of the library call. The source file has the implementation. The tests go through the public interface only, so they would still catch a problem if the internal helpers changed.

`image_export` is the last stage of the pipeline in the README. Every plot gives us a `Canvas`, and `image_export` turns that canvas into a file the user can open.

## AI use

- Tool: Notebook AI
- Purpose: Helped with the PPM and BMP writers, mainly the BMP header bytes and the bottom-up, BGR, row-padded pixel layout.
- Reason: The BMP header fields and pixel order are easy to get wrong from memory. Having a worked example made it easier to follow.

The three files were created by me. AI was used as a supporting tool to help with the code, mostly for the BMP byte layout. I did not copy a finished module. I read the code line by line, watched videos about PPM, BMP and how C++ writes binary files, and I am still learning the details so I can explain and defend each function in the presentation.

# Week 2

## Files added

### `include/cppviz/font.hpp`

This header declares four functions for drawing text on a canvas: `text_width`, `text_height`, `draw_text` and `draw_text_vertical`. The first two return measurements. The other two draw. They all take a canvas reference and a color, so nothing is copied. `draw_text_vertical` is the rotated version used for y-axis labels.

### `src/font.cpp`

This source file implements the four functions.

The file holds a table of bitmap glyphs. Each glyph is a 5 by 7 grid of pixels stored as 7 bytes. Each byte is one row of 5 pixels, and the 5 pixels use bits 4 down to 0. So `0x0E` is `01110` in binary, meaning the middle three pixels of that row are on and the two on the ends are off.

The table covers ASCII 32 (space) up through 95 (underscore). That is 64 characters in one array. Lowercase letters do not get their own glyphs. Instead `lookup_glyph` maps `a` to `z` onto `A` to `Z` by subtracting 32 from the character code. Characters outside the table get a hollow box so the text still has a shape.

`draw_text` walks through the string one character at a time. For each character it calls `lookup_glyph`, then reads the 7 bytes. For every bit that is set, it fills a small square on the canvas that is `scale` by `scale` pixels. A scale of 2 draws every pixel as a 2 by 2 block, which makes the text twice as big. The gap between characters is one pixel multiplied by scale.

`draw_text_vertical` does the same thing but swaps the axes, so the text reads from bottom to top. It is used for the y-axis label.

### `tests/test_font.cpp`

This test file checks four things.

First, the measurements. `text_width("")` is 0, `text_width("A")` is 6, `text_height(1)` is 7, and scaling doubles both.

Second, the exact placement of the letter `I`. The test draws a single `I` and then checks that the stem lands on the right column and that the two outer columns and the gap column stay empty. That way the tests fail if the bit-reading is off by one.

Third, empty strings. Drawing an empty string on a canvas should change nothing.

Fourth, vertical text. After drawing "AB" rotated, everything outside its bounding box stays background. That confirms the rotation math puts the pixels in the right place.

## How the files work together

The header is the public interface. Anything that needs to draw text includes it and calls the four functions. The source file has the glyph data and the drawing loops. The tests go through the public interface only, so they would still pass if the internals changed.

`font` is what the rest of the library uses for axis labels, titles and tick numbers. Without it every plot would be a picture with no text.

## AI use

- Tool: Notebook AI
- Purpose: Helped build the 5x7 glyph table and the bit-reading loop inside draw_text.
- Reason: Typing 64 glyphs by hand would have taken hours, and the bit-shifting logic for the rotated version was tricky to work out from scratch.

I typed the files into the project myself. AI was used as a helper, not to produce the whole module. I read through the code line by line, watched videos on bitmap fonts and on how C++ writes bits, and I can explain what every function does and why.

# Week 3

## Files added

### `include/cppviz/theme.hpp`
This header defines the `Theme` struct, which holds colors and sizes for the plot background, axes, grid, and text. It also declares four preset functions.

### `src/theme.cpp`
This file implements the four presets. `theme_white` and `theme_whitegrid` are light. `theme_darkgrid` uses a light grey plot area with white grid lines. `theme_dark` is for dark mode.

### `tests/test_theme.cpp`
This file checks that the presets are set up correctly. It verifies the grid settings, checks specific colors, and makes sure the dark theme is actually darker than the white theme.

## How the files work together
The rest of the library uses these themes to style the plots. By passing a Theme into the drawing functions, we can change how the axes, grids, and text look without touching the drawing code itself.

## AI use
- Tool: Notebook AI
- Purpose: Helped write the comments in the header file and the complex code.
- Reason: Wanted the struct to be well-documented. 
