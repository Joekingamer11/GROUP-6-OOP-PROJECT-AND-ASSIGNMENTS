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
```