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

# Week 3 Progress Report

## My Week 3 Files and Modifications

### `include/cppviz/stats.hpp`
This header continues to expose the statistics API from the earlier weeks and now documents the Week 3 bootstrap confidence-interval API. `bootstrap_ci` accepts a data vector, confidence level, number of bootstrap resamples, and random seed, with defaults of `0.95`, `1000`, and `42`. It returns the lower and upper bounds in the shared `Interval` struct.

### `src/stats.cpp`
The Week 3 update to `bootstrap_ci` validates that the input is non-empty, the confidence level is strictly between `0` and `1`, and the number of resamples is positive. It returns the observed value for a one-element input, uses a seeded `std::mt19937` generator to make resampling repeatable, and computes percentile bounds from the bootstrap sample means. The implementation reserves space for all bootstrap means in advance and reuses the reciprocal of the sample size when calculating each mean to reduce repeated allocation and division.

**Note:** In the current working version, the Week 1 and Week 2 function definitions in this source file are represented by a placeholder comment rather than implementation code. Those definitions need to be present for the earlier statistics API to link correctly.

### `include/cppviz/pointplot.hpp`
This new public header defines `PointplotOptions`, including the plot colour, whether confidence intervals and connecting lines are shown, confidence level, marker radius, line thickness, and error-bar cap size. It declares `pointplot`, which takes axes, a dataset, categorical and numeric column names, and optional display settings.

### `src/pointplot.cpp`
This new implementation validates the dataset and column types, groups numeric observations by category, and calculates each category's mean. When confidence intervals are enabled, it calls `bootstrap_ci` for each group. It configures categorical and padded linear scales, draws the plot frame, and renders connecting lines, confidence-interval bars with caps, and circular mean markers. Invalid confidence levels and datasets without usable groups are reported with the library's exceptions.

### `tests/tests_pointplot.cpp`
This new test file exercises point-plot rendering with and without confidence intervals and connecting lines. It checks that rendering changes the canvas and verifies that missing columns, incorrect column types, empty datasets, and invalid confidence levels raise the expected exceptions.

### How these files work together
The point-plot header exposes the options and function to library users. The implementation uses dataset grouping and the statistics API to compute category means and bootstrap confidence intervals, then draws the results on the supplied axes. The new tests cover both rendered output and invalid-input behavior. The Week 3 bootstrap changes in the statistics module provide the confidence intervals used by the plot.

# Week 4 Progress Report

## My Week 4 Files and Modifications

### `data/input/measurements_sample.csv`
This new sample dataset contains 30 observations across four numeric measurements: `length`, `width`, `height`, and `weight`. The values are correlated, making the file useful as example input for calculating and visualizing relationships between measurements.


### `tests/test_swarmplot_layout.cpp`
This new test file exercises the `swarm_layout` helper independently of the swarmplot renderer. It checks that points in the same category stay at least two marker radii apart, verifies the number of positions for categories with one observation each, and confirms that empty data and a missing category column raise `DataError`.

### `include/cppviz/stats.hpp`
This existing public header now documents the statistics API with Doxygen comments. The descriptions cover the univariate functions, covariance and correlation, correlation-matrix generation, and bootstrap confidence intervals, including their parameters, return values, and relevant error conditions. The function declarations and statistical behavior are unchanged.

### `include/cppviz/pointplot.hpp`
This existing header now documents `PointplotOptions` and the `pointplot` function. The comments explain the visual options and describe the expected columns and possible input errors, making the public point-plot API clearer to library users.

### `src/pointplot.cpp`
The implementation has been reorganized into clearly identified stages for input validation, category statistics, scale setup, frame drawing, and mark rendering. The plotting logic remains the same; the error message for data with no usable groups was clarified to refer to “valid observation groups.”

### How these files work together
The sample CSV provides correlated measurements for visualization examples, while the heatmap example shows a complete annotated-heatmap workflow. The swarm-layout tests check spacing and invalid-input behavior in the swarmplot helper. The updated public headers explain the statistics and point-plot APIs, and the implementation’s step labels make its existing rendering flow easier to follow.