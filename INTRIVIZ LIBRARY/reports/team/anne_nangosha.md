# NANGOSHA ANNE MARY 25/U/0721

## WEEK-01

## FILES

- error.hpp
- types.hpp
- error.cpp
- test_error.cpp
- test_types.cpp

## DESCRIPTION OF NEW FILES

- error.hpp: Defines the library's exception hierarchy for handling invalid arguments, data issues, and I/O errors. It also declares the require() helper used to validate inputs and throw InvalidArgument when conditions fail. We got to learn about new hearder files like #pragma once

- types.hpp: Contains core data type definitions used by the visualization library, including Color, PointI, PointD, Rect, and Margins. It also declares equality operators for Color.
- error.cpp: Implements the require() function and Color comparison operators. This file connects the declarations in the header files to working runtime logic for validation and type behavior.
- test_error.cpp: Tests the custom exception system, ensuring that require() throws InvalidArgument for invalid conditions and that all custom errors inherit from the base VizError.
- test_types.cpp: Verifies the default values and equality behavior of the key geometry and color structures used in the library, ensuring the foundational types behave as expected.

## WEEK-02

## FILES

- validation.hpp
- validation.cpp
- test_validation.cpp
- error.hpp

## DESCRIPTION OF CHANGES MADE IN WEEK 2

- validation.hpp: Added the dataset validation interface for checking required columns, numeric/text column types, empty datasets, and size mismatches. This created a reusable validation layer for future data-processing functions.

- validation.cpp: Implemented the validation functions, including require_column(), require_numeric_column(), require_text_column(), require_not_empty(), and require_same_size(). These functions throw DataError or InvalidArgument when data constraints are violated.

- test_validation.cpp: Added tests to verify that the validation helpers correctly accept valid datasets and raise the expected exceptions for missing columns, wrong types, empty datasets, and mismatched sizes.

- error.hpp: Adjusted the custom error definitions to support the validation layer by ensuring the library has consistent exception classes and the require() helper used in input checks.

## WEEK-03

## FILES

- axes.hpp
- axes.cpp
- test_axes.cpp

## DESCRIPTION OF NEW FILES

- axes.hpp: Declares the `Axes` class, which connects a canvas, plot area, theme, and x/y scales.
- axes.cpp: Implements data-to-pixel coordinate mapping and draws the plot frame, optional grid lines, axis lines, ticks, and labels.
- test_axes.cpp: Tests plot-area pixel ranges, scale mapping, errors when scales are unset, and frame drawing.
