# Week 1

## Files added

### `include/cppviz/canvas.hpp`

This header defines the public `cppviz::Canvas` interface. It provides a 2D pixel grid that stores RGB color data and exposes the main operations a user needs: setting dimensions, writing and reading pixels, filling the canvas, and retrieving the raw buffer. This file is the library-facing API for the drawing layer.

### `src/canvas.cpp`

This source file implements the behavior of the `Canvas` class. It validates the canvas size, creates the pixel buffer, fills it with a background color, writes RGB values in row-major order, ignores out-of-bounds drawing requests, throws errors for invalid reads, and exposes the image data for later rendering or export routines.

### `tests/test_canvas.cpp`

This test file verifies the correctness of the canvas implementation. It checks constructor behavior, default background color, pixel read/write round-trips, clipping for invalid coordinates, exception handling, full-canvas fills, invalid dimensions, and the raw memory layout used by the underlying RGB buffer.

## How the files work together

The header declares the public contract for the canvas, the implementation file provides the actual drawing logic, and the test file ensures that the behavior is reliable and consistent. Together, these files provide an essential foundation for the library's future plotting and image-generation features, making it easier to draw shapes, create scenes, and export images.

# Week 2

## Files added

### `include/cppviz/draw.hpp`

This header declares the public drawing functions for `cppviz::Canvas`. It provides functions for drawing lines, rectangle outlines and fills, and circle outlines and fills, including optional line thickness and documented handling of invalid circle radii.

### `src/draw.cpp`

This source file implements the drawing functions. It uses Bresenham's algorithm for lines and the midpoint circle algorithm for circle outlines, while filled shapes are drawn by setting their covered pixels. Rectangle dimensions are normalized when negative, invalid negative circle radii throw `InvalidArgument`, and the Canvas safely clips pixels outside its bounds.

### `tests/test_draw.cpp`

This test file verifies line drawing in different directions, clipping and thickness, rectangle outlines and fills, circle outlines and fills, and handling of zero or negative circle radii.

## How the files work together

The header defines the drawing API, the implementation applies shapes to the pixel grid provided by `Canvas`, and the tests check that the shapes, boundary behavior, and validation work as expected. Together, these files add the basic shape-drawing operations needed to create plots and other graphics.

# Week 3

## Files added

### `include/cppviz/heatmap.hpp`

This header declares the heatmap API and the `HeatmapOptions` configuration. It lets users choose a colormap, enable cell-value annotations and a colorbar, and provide the value range used for color scaling.

### `src/heatmap.cpp`

This source file implements heatmap rendering on an `Axes` object. It maps matrix values to colors from the selected colormap, draws each value as a cell, uses a neutral gray for NaN values, and optionally draws cell annotations and a colorbar. It also checks that the matrix and label dimensions are compatible.

### `tests/test_heatmap.cpp`

This test file checks a 2-by-2 heatmap rendered with the viridis colormap. It samples the minimum and maximum cells to verify that they receive the corresponding endpoint colors.

## How the files work together

The header provides the heatmap options and public function, the implementation uses the axes, scales, drawing, and colormap functionality to render the matrix, and the test checks that values map to the expected colors. Together, these files add heatmap visualization to the library.