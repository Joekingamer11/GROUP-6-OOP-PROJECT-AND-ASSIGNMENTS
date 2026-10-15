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