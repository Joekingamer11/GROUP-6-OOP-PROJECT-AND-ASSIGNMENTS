# Week 4 Progress Report

## Completed
Member contributions:

- **Member 1 (lead), Kimuli Joseph:** Wrote the one-line interface (`quick.hpp` / `quick.cpp`: `quick_swarmplot`, `quick_pointplot`, `quick_heatmap`) and `test_quick.cpp`; finalised the umbrella header; made small fixes to `figure` found while using it; reviewed all the member's pull requests; ran the merges and the feature-freeze check.
- **Member 2, Nangosha Anne Mary:** Wrote `test_invalid_inputs.cpp` (empty, NaN, wrong-type, missing-column and negative-size cases across the modules); replaced home-made checks in `axes.cpp` with the `validation.hpp` helpers; added doc comments to `types.hpp`, `error.hpp`, `validation.hpp` and `axes.hpp`.
- **Member 3, Nakibuule Maria Liz:** Created `data/input/tips_sample.csv` and `example_swarmplot.cpp`; fixed text overflow in `labels.cpp` that the example revealed; added doc comments to `dataset.hpp`, `groupby.hpp` and `labels.hpp`.
- **Member 4, Anna Kagemulo Mtabazi:** Created `data/input/scores_sample.csv`, `example_pointplot.cpp` and `example_annotations.cpp`; fixed clipping bugs in `annotation.cpp`; added doc comments to `csv_loader.hpp`, `summary.hpp` and `annotation.hpp`.
- **Member 5, Rwotolara Sunday Junior:** Wrote `test_swarmplot_layout.cpp` (independent tests of Member 6's swarm layout), `example_heatmap.cpp` and `data/input/measurements_sample.csv`; fixed visual issues in `pointplot.cpp`; added doc comments to `stats.hpp` and `pointplot.hpp`.
- **Member 6, Kwagala Rebecca:** Wrote `test_pointplot_cross.cpp` (independent tests of Member 5's point plot) and `test_scales_integration.cpp` (scales, ticks and Axes agree); sped up the swarm layout for 1000+ points; added doc comments to `scale.hpp`, `ticks.hpp` and `swarmplot.hpp`.
- **Member 7, Amviiko Rebecca Natasha:** Wrote `test_figure_integration.cpp` (draw a figure, save it, read the BMP header back) and `test_colorbar_heatmap.cpp`; made `heatmap.cpp` skip labels that do not fit; added doc comments to `canvas.hpp`, `draw.hpp` and `heatmap.hpp`.
- **Member 8, Kalule Wegna:** Added `save_image` selection by extension and `write_png` to `image_export`, moved shared byte-writing code into one helper, and extended `test_image_export.cpp`; added doc comments to `font.hpp`, `theme.hpp` and `image_export.hpp`.
- **Member 9, Kadama Humphrey Arthur:** Wrote `example_dashboard.cpp` (one dataset, three images); fixed margin and overflow issues in `colorbar.cpp` that the dashboard revealed; added doc comments to `colormap.hpp` and `colorbar.hpp`.
- **Member 10, Kalema Titus Larry:** Wrote `test_pipeline.cpp` (load, clean, group, summarise), `test_large_data.cpp` (10,000 rows in a few seconds) and `example_themes.cpp` (the same plot in four themes); fixed placement problems in `legend.cpp`; added doc comments to `preprocess.hpp`, `text_util.hpp` and `legend.hpp`.

Team results:
- A fresh clone builds with `cmake -S . -B build` and `cmake --build build`.
- `ctest` runs and N tests pass.
- every example runs and writes its image into `data/output/`.
- A user can make a chart in three lines with the `quick_*` functions.


## Challenges/Blockers
- PNG creation was harder than expected

## Next Week
- Week 5 (2 to 5 November) is a freeze week: no new features, only bug fixes (`fix(...)`), the README and the reports. The README is hand-typed: each member writes their own section from what they built.
  - Member 1: "How we collaborated as a team" section; consolidate the final report; tag `v1.0.0`; one fresh-machine test on Windows
  - Member 2: "Requirements and dependencies" and "Installation and build instructions"; fresh-machine test on a computer that never had the project
  - Member 3: "Project name and description" and "Project objectives"
  - Member 4: "Main features"
  - Member 5: "How to use the library"
  - Member 6: "How to run the tests"; run the whole suite on the final tag and save the output for the final report
  - Member 7: "Examples"; run every example and commit the sample images
  - Member 8: "Project structure"
  - Member 9: "Contributions by group members"; collect the AI use declaration
  - Member 10: "Limitations"; proofread all README sections and check steps against the code
  - Final presentation: every member prepares one slide to explain.

## AI Use
- Tool: Claude (Anthropic) & Microsoft Copilot
- Purpose: Polishing the final code.
- Reason: Running out of time.