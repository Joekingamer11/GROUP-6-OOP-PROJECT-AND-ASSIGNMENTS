# Week 2 Progress Report

## Completed
Member contributions :

- **Member 1 (lead), Kimuli Joseph:**  Wrote `CMakeLists.txt` checks, `src/version.cpp` and the umbrella header `intriviz.hpp`; reviewed every member's pull requests; merged the week's pull requests in dependency order.
- **Member 2, Nangosha Anne Mary:**  Wrote `types.hpp` (`Color`, `PointI`, `PointD`, `Rect`, `Margins`) and `error.hpp` / `error.cpp` (exception classes and `require()`), with tests.
- **Member 3, Nakibuule Maria Liz:**  Wrote the `Dataset` class (`dataset.hpp` / `dataset.cpp`): numeric and text columns, accessors that throw `DataError`, `select_rows`; wrote `test_dataset.cpp`.
- **Member 4, Anna Kagemulo Mtabazi:**  Wrote the CSV loader (`csv_loader.hpp` / `csv_loader.cpp`): `parse_csv`, `load_csv`, quote handling, type detection, missing cells; wrote `test_csv_loader.cpp`.
- **Member 5, Rwotolara Sunday Junior:**  Wrote the basic statistics (`stats.hpp` / `stats.cpp`): `mean`, `median`, `variance`, `stddev`, `min_value`, `max_value`, `quantile`; wrote `test_stats.cpp`.
- **Member 6, Kwagala Rebecca:**  Wrote the abstract `Scale` class and `LinearScale` (`scale.hpp` / `scale.cpp`) with `map` and `invert`; wrote `test_scale.cpp`.
- **Member 7, Amviiko Rebecca Natasha:**  Wrote the `Canvas` class (`canvas.hpp` / `canvas.cpp`): pixel storage, `set_pixel`, `get_pixel`, `fill`; wrote `test_canvas.cpp`.
- **Member 8, Kalule Wegna:** Wrote the image export functions (`image_export.hpp` / `image_export.cpp`): `write_ppm`, `write_bmp`, `save_image`; wrote `test_image_export.cpp`.
- **Member 9, Kadama Humphrey Arthur:** Wrote the colour module (`colormap.hpp` / `colormap.cpp`): hex parsing, `blend`, the "deep" palette and the `Colormap` class; wrote `test_colormap.cpp`.
- **Member 10, Kalema Titus Larry:**  Wrote the test helper `mini_test.hpp` and the text utilities (`text_util.hpp` / `text_util.cpp`: `trim`, `split`, `to_lower`, `try_parse_double`) with tests.

Team results :
- A fresh clone builds with `cmake -S . -B build` and `cmake --build build`.
- `ctest` runs and N tests pass.

## In Progress
- Confirmation of the generation of a bmp image after running tests.
- Suggestion and Implementation of various example files for future tests.

## Challenges/Blockers
- A lot of errors post merging of the code.
- Heavy reliance on AI to solve errors encountered due to schedule clashes with each member's personal time.
- Minor merge conflicts that were later rectified.

## Next Week
- Week 2 coding (12 to 18 October), one segment per member:
  - Member 1: `CategoricalScale`, first example program
  - Member 2: `validation` helpers
  - Member 3: `group_by`
  - Member 4: `describe` summary table, missing-value tokens in the CSV loader
  - Member 5: correlation and bootstrap confidence intervals
  - Member 6: tick marks and `LogScale`
  - Member 7: line, rectangle and circle drawing
  - Member 8: bitmap font
  - Member 9: named colormaps and contrast colour
  - Member 10: preprocessing functions
- Pull requests by Thursday, reviews on Friday, merges before Sunday.

## AI Use
- Tool: Claude (Anthropic) & Microsoft Copilot
- Purpose: Error Rectification and Code Breakdown
- Reason: Some errors were beyond our means of rectification and required more time to resolve that we did not have.