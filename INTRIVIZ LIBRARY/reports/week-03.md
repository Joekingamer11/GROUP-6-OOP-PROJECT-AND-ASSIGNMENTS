# Week 3 Progress Report

## Completed
Member contributions:

- **Member 1 (lead), Kimuli Joseph:**  Wrote `CategoricalScale` (`categorical_scale.hpp` / `categorical_scale.cpp`) and `test_categorical_scale.cpp`; wrote the first example, `example_load_and_describe.cpp`; added the new headers to the umbrella header `intriviz.hpp`; reviewed all member's pull requests.
- **Member 2, Nangosha Anne Mary:**  Wrote the `validation` helpers (`validation.hpp` / `validation.cpp`: `require_column`, `require_numeric_column`, `require_text_column`, `require_not_empty`, `require_same_size`) with `test_validation.cpp`; added a comment to `error.hpp` on which exception to throw when.
- **Member 3, Nakibuule Maria Liz:**  Wrote `unique_values` and `group_by` (`groupby.hpp` / `groupby.cpp`) with `test_groupby.cpp`; fixed issues in `dataset` found in the Week 1 review.
- **Member 4, Anna Kagemulo Mtabazi:**  Wrote the `describe` summary table (`summary.hpp` / `summary.cpp`: `ColumnSummary`, `describe`, `to_string`) with `test_summary.cpp`; added `missing_tokens` (NA, NaN, null) to the CSV loader and its tests.
- **Member 5, Rwotolara Sunday Junior:**  Added `covariance`, `correlation`, `correlation_matrix` and `bootstrap_ci` to `stats.hpp` / `stats.cpp`, and extended `test_stats.cpp`.
- **Member 6, Kwagala Rebecca:**  Wrote `ticks.hpp` / `ticks.cpp` (`nice_ticks`, `log_ticks`, `format_tick`, `padded_range`) and `LogScale`; upgraded `LinearScale::tick_values` and `Scale::tick_label` to use the new tick functions; extended `test_scale.cpp` and wrote `test_ticks.cpp`.
- **Member 7, Amviiko Rebecca Natasha:**  Wrote the drawing functions (`draw.hpp` / `draw.cpp`: Bresenham `draw_line`, `draw_rect`, `fill_rect`, `draw_circle`, `fill_circle`) with `test_draw.cpp`; fixed issues in `canvas` found in the Week 1 review.
- **Member 8, Kalule Wegna:**  Wrote the bitmap font (`font.hpp` / `font.cpp`: glyph table, `text_width`, `text_height`, `draw_text`, `draw_text_vertical`) with `test_font.cpp`; fixed issues in `image_export` found in the Week 1 review.
- **Member 9, Kadama Humphrey Arthur:**  Added `make_colormap` (viridis, magma, coolwarm, gray), `palette_color`, `luminance` and `contrast_text_color` to `colormap.hpp` / `colormap.cpp`, and extended `test_colormap.cpp`.
- **Member 10, Kalema Titus Larry:**  Wrote the preprocessing functions (`preprocess.hpp` / `preprocess.cpp`: `drop_missing`, `fill_missing`, `filter_rows`, `select_columns`, `normalize_minmax`, `standardize`) with `test_preprocess.cpp`; made small fixes to `text_util` and `mini_test.hpp` that teammates asked for.

Team results:
- A fresh clone builds with `cmake -S . -B build` and `cmake --build build`.
- `ctest` runs and N tests pass.

## In Progress
- Better pogrammes that use this code to showcase its abilities.

## Challenges/Blockers
- Floating-point rounding in `nice_ticks` among other small errors during library building.


## Next Week
- Week 4 coding (19 to 25 October), one segment per member:
  - Member 1: `Figure` class
  - Member 2: `Axes` class (plot frame, scales, ticks)
  - Member 3: title and axis labels
  - Member 4: annotations (text, reference lines, arrows)
  - Member 5: point plot
  - Member 6: swarm plot and its layout algorithm
  - Member 7: heatmap
  - Member 8: themes
  - Member 9: colorbar
  - Member 10: legend
- Pull requests by Thursday, reviews on Friday, merges on Saturday.

## AI Use
- Tool: Claude (Anthropic) & Microsoft Copilot
- Purpose: Troubleshooting code and Creation of suitable example programme for the week.
- Reason: Errors were insanely difficult to rectify on our own and we didn't have suggestions for a sample of our work for this week.
