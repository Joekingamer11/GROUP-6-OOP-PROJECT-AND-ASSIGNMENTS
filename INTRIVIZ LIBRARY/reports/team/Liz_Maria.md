# Week 1

## Files added

### `include/cppviz/dataset.hpp` and `src/dataset.cpp`

These files add the `cppviz::Dataset` class for storing and working with tabular data containing numeric and text columns. It supports column queries and access, row selection, and validation of column names, lengths, and types. The `is_missing()` helper identifies NaN values.

### `tests/test_dataset.cpp`

This test file verifies Dataset creation, column operations, data access, error handling, missing-value detection, and row selection.


## How the files work together

The public headers expose the version and Dataset interfaces, and the corresponding source files implement them. CMake compiles the source files into the library, while `test_dataset.cpp` checks the Dataset behavior.

# Week 2

## Files added to the repository

### `include/cppviz/dataset.hpp`
This header defines the `cppviz::Dataset` class, including support for column names, row access, validation, and tabular data management.

### `src/dataset.cpp`
This source file implements the dataset logic for working with numeric and text columns, row selection, missing-value detection, and error handling.

### `tests/test_dataset.cpp`
This file contains tests covering dataset creation, column operations, row selection, missing values, and error conditions.

# Week 3

## Files added to the repository

### `include/cppviz/labels.hpp`
This public header declares the plot-labeling functions `draw_title`, `draw_xlabel`, and `draw_ylabel` for an `Axes` object.

### `src/labels.cpp`
This source file implements the labeling functions. Titles and x-axis labels are centered, y-axis labels are drawn vertically, empty labels are ignored, and text is reduced to the minimum font scale when needed to fit.

### `tests/test_labels.cpp`
This test file checks that empty labels do not draw, labels appear in the expected canvas regions, and long titles are handled.

These files add and test plot-labeling support for titles and axis labels.