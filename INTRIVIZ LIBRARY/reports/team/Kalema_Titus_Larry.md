# Week 1
## Files added

### `tests/mini_test.hpp`

This header provides reusable test macros for checking conditions, equality, numerical closeness, and exceptions. It also provides `finish()`, which reports the total number of test failures and returns `0` when all tests pass.

### `include/cppviz/text_util.hpp`

This header declares string-processing utilities for trimming whitespace, splitting text, converting text to lowercase, and parsing numeric values.

### `src/text_util.cpp`

This source file implements the string-processing utilities declared in the corresponding header.

# Week 2 Changes
## Files added

### `include/cppviz/preprocess.hpp`

This header declares preprocessing utilities for dataset cleaning and feature scaling, including dropping rows with missing values, filling missing numeric values, filtering rows by a predicate, selecting columns, min-max normalization, and z-score standardization.

### `src/preprocess.cpp`

This source file implements the preprocessing functions described in the header. It handles missing-value filtering, value replacement, row filtering, column selection, min-max normalization, and standardization while validating inputs and raising `DataError` for invalid usage.

### `tests/test_preprocess.cpp`

This test file validates the preprocessing logic through unit tests for dropping missing rows, filling missing values, filtering rows, selecting columns, min-max normalization, and z-score standardization.
