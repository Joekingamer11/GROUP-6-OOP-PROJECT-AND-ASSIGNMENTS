 # Week 1 

## Files added

### `include/cppviz/stats.hpp`

This is the main public header for the statistics module. It declares the core functions like `mean`, `variance`, `stddev`, `min_value`, `max_value`, `quantile`, and `median`. It also defines the `Interval` struct for confidence intervals and pulls in the shared error headers. Think of it as the central hub that clients use to access these common statistical tools.

### `src/stats.cpp`

This file is where the actual math happens. It implements everything declared in the header, handling the calculations for central tendency and data spread. It also makes sure the inputs are safe—for example, it throws a `DataError` if someone passes an empty vector, or an `InvalidArgument` if they ask for a quantile outside the 0 to 1 range.

### `tests/test_stats.cpp`

This is the test harness for the stats module. It runs the functions against normal cases with hand-computed values, checks edge cases like single-element vectors, and makes sure the code throws the right exceptions when given invalid inputs. It's essentially a safety net to confirm everything works as expected.

## How the files work together

The header exposes the statistical functions to anyone using the library, while `stats.cpp` contains the actual logic behind them. The test file depends on both to verify correctness. When the project is built, the implementation in `src/` gets compiled into the library based on the CMake configuration.

# Week 2 Progress Report

## My Week 1 Files and Modifications

### `include/cppviz/stats.hpp`
This is the main public header for the statistics module. It declares the core functions I implemented, including `mean`, `variance`, `stddev`, `min_value`, `max_value`, `quantile`, and `median`. It also defines the `Interval` struct for confidence intervals and includes the shared error headers. This file acts as the public API for the statistical tools used across the project.

### `src/stats.cpp`
This file contains the actual implementation of the statistical calculations. It handles the core numeric logic for measuring central tendency and spread, and also validates input values. For example, it throws a `DataError` when an empty dataset is passed and raises an `InvalidArgument` error if a quantile value falls outside the valid `0` to `1` range.

### `tests/test_stats.cpp`
This test file verifies the correctness of the statistics module. It tests normal calculations, edge cases such as single-element inputs, and failure conditions for invalid arguments. It ensures the implemented functions behave consistently and return the expected results under both valid and invalid conditions.

### How these files work together
The header exposes the statistical API, the implementation file performs the calculations and checks, and the tests confirm the behavior. Together, these files provide a complete and reusable stats module that integrates with the wider INTRIVIZ library.
```