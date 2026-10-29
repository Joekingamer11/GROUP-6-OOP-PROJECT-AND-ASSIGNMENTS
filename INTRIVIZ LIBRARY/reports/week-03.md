# Week 3 Progress Report

## Completed
Member contributions:

- **Member 1 (lead), Kimuli Joseph:** Wrote the `Figure` class (`figure.hpp` / `figure.cpp`: canvas owned through a `unique_ptr`, margins, `axes()`, `canvas()`, `set_margins()`, `save()`) and `test_figure.cpp`; updated the umbrella header with the new headers; reviewed every member's pull request; merged the week's pull requests in the agreed order.
- **Member 2, Nangosha Anne Mary:** Wrote the `Axes` class (`axes.hpp` / `axes.cpp`: `to_pixel`, x and y pixel ranges, scale setters and getters, `draw_frame` with background, grid, axis lines, tick marks and tick labels) and `test_axes.cpp`;
- **Member 3, Nakibuule Maria Liz:** Wrote the label functions (`labels.hpp` / `labels.cpp`: `draw_title`, `draw_xlabel`, `draw_ylabel`) and `test_labels.cpp`;
- **Member 4, Anna Kagemulo Mtabazi:** Wrote the annotation functions (`annotation.hpp` / `annotation.cpp`: `annotate_text`, `annotate_hline`, `annotate_vline`, `annotate_arrow`) and `test_annotation.cpp`;
- **Member 5, Rwotolara Sunday Junior:** Wrote the point plot (`pointplot.hpp` / `pointplot.cpp`: `PointplotOptions`, group means, bootstrap confidence intervals, lines, interval bars and markers) and `test_pointplot.cpp`;
- **Member 6, Kwagala Rebecca:** Wrote the swarm plot (`swarmplot.hpp` / `swarmplot.cpp`: `SwarmplotOptions`, the non-overlapping `swarm_layout`, dot drawing with category colours) and `test_swarmplot.cpp`; fixed issues in `scale` and `ticks` found while plotting;
- **Member 7, Amviiko Rebecca Natasha:** Wrote the heatmap (`heatmap.hpp` / `heatmap.cpp`: `HeatmapOptions`, cell colours from a colormap, optional numbers in the cells) and `test_heatmap.cpp`; fixed issues in `draw` found while drawing cells;
- **Member 8, Kalule Wegna:** Wrote the themes (`theme.hpp` / `theme.cpp`: the `Theme` struct and the white, whitegrid, darkgrid and dark presets) and `test_theme.cpp`; fixed spacing and missing characters in `font` found on real tick labels; reviewed Member 7's pull request.
- **Member 9, Kadama Humphrey Arthur:** Wrote the colorbar (`colorbar.hpp` / `colorbar.cpp`: `draw_colorbar` with gradient strip and min, middle and max labels) and `test_colorbar.cpp`;
- **Member 10, Kalema Titus Larry:** CONFIRM. Wrote the legend (`legend.hpp` / `legend.cpp`: `draw_legend` with coloured squares and names) and `test_legend.cpp`; fixed issues in `preprocess` found when testing with real CSV files;

Team results (keep only what is true):
- A fresh clone builds with `cmake -S . -B build` and `cmake --build build`.
- `ctest` runs and N tests pass.
- A swarm plot, a point plot and a heatmap were each saved as an image and looked at by the team on Saturday.

## In Progress
- first images look rough; list the visual problems still to fix (overlapping labels, margins too small, colours).

## Challenges/Blockers
- CONFIRM: describe real problems, for example plots depending on `Axes` and `Theme` headers that landed late, a `Figure` that crashed after being moved, text that did not fit in the margin, merge conflicts.
- The library name is INTRIVIZ but the header folder and namespace is still `cppviz`(We decided to keep the header file the same).

## Next Week
- Week 4 coding (26 October to 1 November), one segment per member:
  - Member 1: `quick_*` one-line plot functions and the final umbrella header
  - Member 2: invalid-input tests across all modules and header comments
  - Member 3: swarm plot example and the tips sample CSV
  - Member 4: point plot and annotation examples and the scores sample CSV
  - Member 5: independent tests of the swarm layout, heatmap example and measurements CSV
  - Member 6: independent tests of the point plot, scale and axes integration tests
  - Member 7: whole-figure and heatmap-with-colorbar tests
  - Member 8: PNG export (stretch goal) and `save_image` by extension
  - Member 9: dashboard example (three plots)
  - Member 10: pipeline test, 10,000-row stress test and themes example
- Feature freeze on Sunday 1 November at 18:00: after that only bug fixes and README writing.
- Pull requests by Thursday, reviews on Friday, merges on Saturday.

## AI Use
- Tool: Claude (Anthropic) & Microsoft Copilot
- Purpose: Example creation.
- Reason: We simply did not have time to do some tasks like creating the examples.