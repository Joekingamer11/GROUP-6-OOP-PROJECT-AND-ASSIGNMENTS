# Member 9 (Humphrey Arthur) — Week 1 Report

## Files Created
* include/cppviz/colormap.hpp: Public header declaring color conversion functions (color_from_hex, color_to_hex), blend(), categorical_palette(), and the Colormap class.
* src/colormap.cpp: Source implementation for hex parsing/formatting, clamped linear blending, Seaborn "deep" palette generation, and multi-stop gradient sampling.
* tests/test_colormap.cpp: CTest executable verifying hex conversions, boundary clamping, palette contents, and exception paths.

## What Member 9 Added
Member 9 established the colormap module for Week 1 ("Foundations & Colour Infrastructure"), providing the core color parsing, blending, and gradient sampling engine for the cppviz library.

## How the Code Works
1. Hex Conversion: Extracts RGB bytes from 6-digit hex strings ("#RRGGBB") using bitwise operations, formatting Color structs back to hex strings. Invalid strings throw InvalidArgument.
2. Linear Interpolation: blend() computes weighted color steps (a + (b - a) * t), clamping t in [0.0, 1.0].
3. Palettes & Gradients: categorical_palette("deep") returns 10 discrete Seaborn colors, while Colormap::at(t) maps normalized t values across color stops to interpolate continuous gradients.