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
