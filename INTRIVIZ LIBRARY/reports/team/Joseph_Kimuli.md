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