# Week 1

## Files added

### `include/cppviz/cppviz.hpp`

The public `cppviz` header declares the library's `cppviz::version()` function. It also includes the shared types and error headers, providing a central header for clients that need these common library interfaces.

### `src/version.cpp`

This source file implements `cppviz::version()`. It returns the library version as the string `"1.0.0"`, providing a single version value that can be queried by library users.

## How the files work together

The header exposes the version function to consumers of the library, while `version.cpp` supplies its implementation. The implementation is compiled into the library from `src/` by the project's CMake configuration.

# Week 2

## Files added

### `include/cppviz/categorical_scale.hpp`

This header declares `CategoricalScale`, which extends the shared `Scale` interface for mapping discrete category names to pixel positions. It provides category lookup, tick generation, and band-width helpers for plotting categorical axes.

### `src/categorical_scale.cpp`

This source file implements `CategoricalScale`. It validates the input categories, maps category indices to pixel positions, inverts positions back into category values, and returns tick labels for each category.

### `include/cppviz/cppviz.hpp`

I adjusted the umbrella header to include `cppviz/categorical_scale.hpp`, so the new scale is available through the main library header used by project consumers.

## How the files work together

The categorical scale header exposes the public interface, while `categorical_scale.cpp` supplies its implementation. The umbrella header includes the new scale alongside the other shared module interfaces so library users access the feature through the central `cppviz.hpp` entry point.

# Week 3

## Files added

### `include/cppviz/figure.hpp`

This header declares the `Figure` class, a high-level plot container that owns a `Canvas` and provides access to its main `Axes`. It supports custom plot margins, themes, and saving the figure as an image.

### `src/figure.cpp`

This source file implements `Figure`. It creates the canvas and axes, validates that margins leave enough space for the plot area, updates the axes when margins change, and saves the canvas through the image-export functionality.

### `tests/test_figure.cpp`

This test file covers default and custom figure dimensions and margins, invalid margin handling, move semantics, and saving the figure to an image file.

## Files modified

### `include/cppviz/cppviz.hpp`

I updated the umbrella header to include `cppviz/figure.hpp`, making `Figure` available to consumers through the central library header.

## How the files work together

The public header exposes the `Figure` interface, while `figure.cpp` implements its canvas ownership, axes layout, and image saving. The tests verify the main behaviors, and the umbrella header makes the new class available through the library's central include.

# Week 4

## Files added

### `include/cppviz/quick.hpp`

This header declares the single-call plotting helpers `quick_swarmplot()`, `quick_pointplot()`, and `quick_heatmap()`. These convenience functions create fully rendered `Figure` objects from a dataset and plot specification without requiring callers to assemble the plotting pipeline manually.

### `src/quick.cpp`

This source file implements the quick plotting API. It validates the dataset columns, creates a standard `Figure`, renders the appropriate plot on the main axes, adds a title and axis labels where needed, and returns the finished figure ready for saving or display.

## Files modified

### `include/cppviz/cppviz.hpp`

I updated the umbrella header to expose the quick plotting API alongside the project’s other public modules, so the new helpers are available through the central `cppviz.hpp` include.

## How the files work together

The quick helpers act as a front-end over the lower-level plotting stack: they validate the dataset, create a `Figure`, delegate to `swarmplot`, `pointplot`, or `heatmap`, then add labels and the final layout. This makes it easier for library users to generate common plot types from a single function call.