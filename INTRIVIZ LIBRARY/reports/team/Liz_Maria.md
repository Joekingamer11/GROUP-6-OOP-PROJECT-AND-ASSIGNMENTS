# Week 1

## Files added

### `include/cppviz/dataset.hpp` and `src/dataset.cpp`

These files add the `cppviz::Dataset` class for storing and working with tabular data containing numeric and text columns. It supports column queries and access, row selection, and validation of column names, lengths, and types. The `is_missing()` helper identifies NaN values.

### `tests/test_dataset.cpp`

This test file verifies Dataset creation, column operations, data access, error handling, missing-value detection, and row selection.


## How the files work together

The public headers expose the version and Dataset interfaces, and the corresponding source files implement them. CMake compiles the source files into the library, while `test_dataset.cpp` checks the Dataset behavior.