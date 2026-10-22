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


# Member 9 (Humphrey Arthur) — Week 2 Report

## Files Updated
* include/cppviz/colormap.hpp: Added declarations for make_colormap(), palette_color(), luminance(), and contrast_text_color().
* src/colormap.cpp: Implemented color stops for viridis, magma, coolwarm, and gray, palette modulo wrap-around indexing, perceived brightness calculations, and background contrast logic.
* tests/test_colormap.cpp: Added unit tests verifying named colormap endpoints, palette indexing bounds, luminance accuracy, and black/white contrast text selection.

## What Member 9 Added
Member 9 expanded the colormap module for Week 2 ("Data Processing and Drawing Tools"), delivering predefined color gradients, safe palette indexing, and text contrast helpers for plot annotations and heatmaps.

## How the Code Works
1. Named Colormaps: make_colormap() builds Colormap instances configured with color stops for viridis, magma, coolwarm, and gray.
2. Wrap-Around Indexing: palette_color() safely retrieves palette colors using index % palette.size(), throwing InvalidArgument if the vector is empty.
3. Accessibility & Contrast: luminance() measures perceived brightness (0.299*R + 0.587*G + 0.114*B), and contrast_text_color() automatically returns white for dark backgrounds and black for light ones.