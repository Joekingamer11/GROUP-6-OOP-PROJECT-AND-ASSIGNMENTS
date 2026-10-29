# INTRIVIZ: A C++ Data Visualization Library

#### Group 6 Members
| No. | Name | Course | Role |
| ------ | ------ | ------ | ------ |
| 1 (Lead) | KIMULI JOSEPH | BCCE | Lead and Integrator |
| 2 | NANGOSHA ANNE MARY | BBIO | Foundations and Axes |
| 3 | NAKIBUULE MARIA LIZ | BBIO | Data Structures and Labels |
| 4 | ANNA KAGEMULO MTABAZI | BELE | Data In, Summaries, Notes |
| 5 | RWOTOLARA SUNDAY JUNIOR | BBIO | Statistics and Point Plots |
| 6 | KWAGALA REBECCA | BELE | Scaling and Swarm Plots |
| 7 | AMVIIKO REBECCA NATASHA | BCCE | Pixels, Shapes, Heatmaps |
| 8 | KALULE WEGNA | BBIO | Text, Themes, Image Files |
| 9 | KADAMA HUMPHREY ARTHUR | BBIO | Color Infrastructure & Dashboard |
| 10 | KALEMA TITUS LARRY | BBIO | Test Tools, Text Helpers, Cleaning |

---

### Objective
**Main aim:** A reusable, dependency-free C++17 library—not a standalone executable—that turns tabular CSV data into statistical charts in the spirit of Python's Seaborn.

**User Experience:** Load a CSV file, clean and summarize the dataset, render statistical plots (swarm plot, point plot with confidence intervals, or correlation heatmap), and save the image output in just a few lines of C++ code.

**Learning & Technical Goals:**
* **Object-Oriented C++:** Polymorphic scale hierarchies (`Scale`, `LinearScale`, `LogScale`, `CategoricalScale`), encapsulated heap canvas management (`Figure`, `Canvas`), and custom exception hierarchies (`VizError`).
* **Software Drawing Engine:** Pure C++ pixel rendering engine handling 2D raster grids, Bresenham line drawing, circle/rectangle fills, 5x7 bitmap font rendering, and 24-bit BMP image serialization.
* **Data & Algorithms:** Robust CSV parsing with quoted field state machines, typed column tables (`Dataset`), statistical metrics (mean, variance, quantiles, Pearson correlation, bootstrap 95% confidence intervals), and non-overlapping swarm dot layout algorithms.
* **Engineering Standards:** Strict separation of public contracts (`include/intriviz/`) and implementations (`src/`), clean exception-based input validation (`require_column`), CMake build system, CTest suite, zero external dependencies, and collaborative Git workflow.

---

### 1. Project Structure

```text
intriviz/
├── CMakeLists.txt                 # Build integration & target definitions (M1)
├── LICENSE                        # MIT License (M1)
├── .gitignore                     # Git exclusion patterns (M1)
├── README.md                      # Complete library documentation (Group 6)
├── include/intriviz/                # PUBLIC Headers (#include <intriviz/...>)
│   ├── Intriviz.hpp                 # Umbrella header including all public modules (M1)
│   ├── types.hpp                  # Color, PointI, PointD, Rect, Margins structs (M2)
│   ├── error.hpp                  # VizError exception class hierarchy (M2)
│   ├── validation.hpp             # Shared dataset & argument guards (M2)
│   ├── dataset.hpp                # Typed tabular Dataset class (M3)
│   ├── csv_loader.hpp             # CSV parsing & file loading options (M4)
│   ├── preprocess.hpp             # Cleaning, filtering, normalization (M10)
│   ├── groupby.hpp                # Category grouping & first-appearance ordering (M3)
│   ├── stats.hpp                  # Summary statistics & bootstrap CIs (M5)
│   ├── summary.hpp                # ColumnSummary & describe() text table (M4)
│   ├── scale.hpp                  # Abstract Scale, LinearScale, LogScale (M6)
│   ├── ticks.hpp                  # Nice tick calculation & label formatting (M6)
│   ├── categorical_scale.hpp      # Category-to-pixel mapping (M1)
│   ├── canvas.hpp                 # 2D RGB pixel storage buffer (M7)
│   ├── draw.hpp                   # Bresenham lines, rectangles, circles (M7)
│   ├── font.hpp                   # 5x7 bitmap font & vertical text (M8)
│   ├── colormap.hpp               # Color palettes, blending, colormaps (M9)
│   ├── image_export.hpp           # PPM, BMP, and PNG export (M8)
│   ├── theme.hpp                  # Visual theme presets (white, dark, grids) (M8)
│   ├── axes.hpp                   # Plot frame, tick marks, grid, data mapping (M2)
│   ├── labels.hpp                 # Titles, x-axis, and y-axis labels (M3)
│   ├── annotation.hpp             # Text notes, reference lines, arrows (M4)
│   ├── colorbar.hpp               # Heatmap gradient strip & legend key (M9)
│   ├── legend.hpp                 # Categorical color legend box (M10)
│   ├── figure.hpp                 # Figure owner wrapping Canvas & Axes (M1)
│   ├── pointplot.hpp              # Grouped means & confidence interval plot (M5)
│   ├── swarmplot.hpp              # Non-overlapping categorical swarm plot (M6)
│   ├── heatmap.hpp                # Matrix heatmap with cell text annotations (M7)
│   └── quick.hpp                  # Three-line quick plotting interface (M1)
├── src/                           # Matching implementation files (.cpp)
│   └── version.cpp                # Library version string definition (M1)
├── tests/                         # CTest unit & integration tests
│   ├── mini_test.hpp              # Dependency-free testing macro framework (M10)
│   └── test_*.cpp                 # Standalone unit test executables (Module Owners)
├── examples/                      # Runnable demonstration programs
│   └── example_*.cpp              # Application examples (Group 6)
├── data/                          # Test fixtures & outputs
│   ├── input/                     # Sample CSV datasets (tips, scores, measurements)
│   └── output/                    # Generated BMP & PNG image outputs (.gitkeep)
└── reports/                       # Weekly progress & final team reports
```

#### Module Groupings & Responsibilities
* **Data Management:** `dataset`, `csv_loader`, `preprocess`, `groupby`, `stats`, `summary` — Ingest CSV text, detect numeric vs text types, clean missing rows, group categories, compute summary statistics and bootstrap confidence intervals, and format text summary tables.
* **Scaling & Coordinate Mapping:** `scale`, `ticks`, `categorical_scale` — Transform continuous data domains or discrete category names into horizontal and vertical canvas pixel coordinates with clean, rounded tick values.
* **Drawing & Rendering Engine:** `canvas`, `draw`, `font`, `colormap`, `image_export` — Provide 2D memory pixel buffers, draw shapes using raster algorithms, render 5x7 bitmap text, interpolate continuous color gradients, and export binary BMP/PPM image files.
* **Layout & Plot Aesthetics:** `theme`, `axes`, `labels`, `annotation`, `colorbar`, `legend`, `figure` — Coordinate plot borders, grid lines, tick labels, titles, reference lines, arrows, colormap legends, category color keys, and canvas lifetime management.
* **Statistical Visualizations:** `swarmplot`, `pointplot`, `heatmap`, `quick` — Generate Seaborn-style swarm plots with collision avoidance, point plots with 95% bootstrap CI error bars, correlation heatmaps, and one-line helper functions.
* **Shared Utilities:** `types`, `error`, `validation`, `text_util`, `mini_test` — Provide shared geometric/color data structures, custom runtime exceptions, validation guards, string trimming/parsing, and lightweight unit test assertions.

---

### 2. Requirements and Dependencies

* **C++ Compiler:** C++17 compliant compiler (GCC 7+, Clang 5+, MSVC 2017+).
* **Build System:** CMake version 3.16 or newer.
* **Version Control:** Git.
* **External Libraries:** **None.** The library is completely self-contained and dependency-free.
* **Image Viewer:** Any viewer supporting 24-bit uncompressed BMP or PPM files (e.g., IrfanView, GIMP, macOS Preview, Windows Photos, Linux eog).
* **CMake Build Switches:**
  * `INTRIVIZ_BUILD_EXAMPLES`: Builds demonstration programs in `examples/` (Default: `ON`).
  * `INTRIVIZ_BUILD_TESTS`: Builds unit test executables in `tests/` (Default: `ON`).

---

### 3. Main Features

* **Data Loading & Parsing:** Ingests CSV files or raw strings with automatic column type inference (numeric vs text), handling quoted fields containing commas, and configurable missing value tokens (`NA`, `NaN`, `null`, blank cells).
* **Data Preprocessing:** Flexible data cleaning including row dropping (`drop_missing`), missing value filling (`fill_missing`), lambda row filtering (`filter_rows`), column selection, min-max scaling, and z-score standardization (`standardize`).
* **Statistical Summaries:** Computes mean, sample variance, standard deviation, median, arbitrary quantiles, Pearson correlation matrices, 95% bootstrap confidence intervals, and outputs aligned text summary tables (`describe()`).
* **Swarm Plots (`swarmplot`):** Displays all individual data points per category, applying an iterative collision-avoidance algorithm that shifts overlapping points sideways without leaking outside category bands.
* **Point Plots (`pointplot`):** Shows category averages as central points connected by lines, accompanied by vertical error bars representing 95% bootstrap confidence intervals with cap bars.
* **Annotated Heatmaps (`heatmap`):** Renders 2D matrices (such as correlation tables) as colored grids using perceptual colormaps (`viridis`, `magma`, `coolwarm`, `gray`), with automated contrast text coloring (black/white) and integrated colorbar scales.
* **Flexible Axis Scaling:** Object-oriented scale classes supporting linear mapping (`LinearScale`), logarithmic mapping (`LogScale`), and discrete categorical mapping (`CategoricalScale`) with automatic "nice" tick selection.
* **Labels, Legends, & Annotations:** Integrated plot titles, horizontal/vertical axis labels, categorical color legends, data-coordinate text notes, reference lines (`hline`, `vline`), and vector arrows.
* **Themes & Canvas Rendering:** Low-level software drawing engine supporting custom themes (`theme_white`, `theme_whitegrid`, `theme_darkgrid`, `theme_dark`), line thickness, circle fills, and scaled 5x7 bitmap text.
* **Image Serialization:** Serializes canvas pixel memory directly into binary uncompressed 24-bit `.bmp` files, `.ppm` files, and `.png` files.

---

### 4. Build the Library

#### Linux and macOS
```bash
# 1. Clone the repository
git clone <repository_url>
cd intriviz

# 2. Configure the build directory using C++17 and CMake
cmake -S . -B build -DINTRIVIZ_BUILD_EXAMPLES=ON -DINTRIVIZ_BUILD_TESTS=ON

# 3. Compile the library, test suite, and examples
cmake --build build -j4
```

#### Windows (Command Prompt / PowerShell with MSVC or MinGW)
```cmd
:: 1. Clone the repository
git clone <repository_url>
cd intriviz

:: 2. Configure the build directory
cmake -S . -B build -DINTRIVIZ_BUILD_EXAMPLES=ON -DINTRIVIZ_BUILD_TESTS=ON

:: 3. Compile in Release mode
cmake --build build --config Release
```

---

### 5. Install the Library (Optional)

#### Linux and macOS
```bash
cmake --install build
```

#### Windows (Command Prompt)
```cmd
cmake --install build --config Release
```

---

### 6. Use the Library in Your Own Program

#### Example Application Code (`main.cpp`)
```cpp
#include <intriviz/Intriviz.hpp>
#include <iostream>

int main() {
    try {
        // 1. Load sample dataset and drop missing entries
        intriviz::Dataset tips = intriviz::load_csv("data/input/tips_sample.csv");
        intriviz::Dataset clean = intriviz::drop_missing(tips);

        // 2. Generate a swarm plot using the three-line quick API
        intriviz::Figure fig = intriviz::quick_swarmplot(
            clean, "day", "total_bill", "Total Bill Distribution by Day"
        );

        // 3. Save the rendered figure to a BMP image file
        fig.save("bill_swarmplot.bmp");
        std::cout << "Successfully generated bill_swarmplot.bmp\n";

    } catch (const intriviz::VizError& e) {
        std::cerr << "intriviz Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
```

#### Compilation & Execution Commands
```bash
# Compile linking against installed intriviz library
g++ -std=c++17 main.cpp -lintriviz -o my_plot_app

# Run executable
./my_plot_app
```

---

### 7. Examples

The library includes six standalone runnable examples located in `examples/`:

1. **`examples/example_load_and_describe.cpp`**
   * **Demonstrates:** CSV loading (`tips_sample.csv`), missing row filtering (`drop_missing`), computing column statistics, and formatting an aligned text summary table.
   * **Output:** Terminal text table output.
2. **`examples/example_week_2.cpp`**
   * **Demonstrates:** CSV parsing, missing-value cleanup, and statistical summaries.
   * **Output Writes:** `data/output/week2_showcase.bmp`
3. **`examples/example_week3_showcase.cpp`**
   * **Demonstrates:** A combined point and swarm plot with summary statistics and confidence intervals.
   * **Output Writes:** `data/output/week3_showcase.bmp`
4. **`examples/example_swarmplot.cpp`**
   * **Demonstrates:** Swarm plot score distributions across two groups.
   * **Output Writes:** `data/output/example_swarmplot.bmp`
5. **`examples/example_pointplot.cpp`**
   * **Demonstrates:** Mean scores across groups with bootstrap confidence intervals.
   * **Output Writes:** `data/output/example_pointplot.bmp`
6. **`examples/example_heatmap.cpp`**
   * **Demonstrates:** An annotated heatmap of correlations between three subjects.
   * **Output Writes:** `data/output/example_heatmap.bmp`

---

### 8. Run the Tests

The test suite is executed using CTest from the build directory. Each module possesses its own standalone test executable in `tests/`:

```bash
# Run all 30 test executables with failure details enabled
ctest --test-dir build --output-on-failure
```

#### Sample Passing Test Output
```text
Test project /path/to/intriviz/build
      Start  1: test_types
 1/30 Test  #1: test_types ........................   Passed    0.01 sec
      Start  2: test_error
 2/30 Test  #2: test_error ........................   Passed    0.01 sec
      Start  3: test_dataset
 3/30 Test  #3: test_dataset ......................   Passed    0.02 sec
      Start  4: test_csv_loader
 4/30 Test  #4: test_csv_loader ...................   Passed    0.02 sec
      ...
      Start 28: test_pipeline
28/30 Test #28: test_pipeline .....................   Passed    0.04 sec
      Start 29: test_invalid_inputs
29/30 Test #29: test_invalid_inputs ...............   Passed    0.03 sec
      Start 30: test_large_data
30/30 Test #30: test_large_data ...................   Passed    0.18 sec

100% tests passed, 0 tests failed out of 30
Total Test time (real) = 0.92 sec
```

---

### 9. Limitations

* **Image Export Formats:** Primary export formats are uncompressed 24-bit BMP and ASCII P6 PPM. PNG export is supported via uncompressed DEFLATE block structures without third-party zlib dependencies.
* **Software Pixel Rendering:** All drawing operations (lines, circles, text, shapes) are software-rendered into 2D CPU memory buffers (`Canvas`). While highly performant for standard statistical figures up to 4K resolution, rendering massive datasets with 100,000+ simultaneous shapes will experience higher CPU layout times compared to hardware-accelerated GPU pipelines.
* **Font System:** Uses a custom built-in 5x7 ASCII bitmap font with integer scale factors (scale 1, 2, 3...). TrueType (`.ttf`) vector font loading and non-ASCII Unicode glyphs are not natively supported.
* **Static Image Library:** Designed specifically as an offline plotting library that renders directly to static image files; it does not open interactive GUI desktop windows or event loops (e.g., GLFW or Qt).

---

### 10. Contributions by Group Members

#### Member 1 (Lead & Integrator) — KIMULI JOSEPH
* **Modules Written:** `CMakeLists.txt`, `src/version.cpp`, `include/intriviz/Intriviz.hpp`, `categorical_scale.hpp`/`.cpp`, `figure.hpp`/`.cpp`, `quick.hpp`/`.cpp`.
* **Tests & Examples:** `test_categorical_scale.cpp`, `test_figure.cpp`, `test_quick.cpp`, `example_load_and_describe.cpp`.
* **Code Reviews:** Reviewed every member's pull requests; managed repository releases and week-by-week build integrations.

#### Member 2 — NANGOSHA ANNE MARY
* **Modules Written:** `types.hpp`, `error.hpp`/`.cpp`, `validation.hpp`/`.cpp`, `axes.hpp`/`.cpp`.
* **Tests & Examples:** `test_types.cpp`, `test_error.cpp`, `test_validation.cpp`, `test_axes.cpp`, `test_invalid_inputs.cpp`.

#### Member 3 — NAKIBUULE MARIA LIZ
* **Modules Written:** `dataset.hpp`/`.cpp`, `groupby.hpp`/`.cpp`, `labels.hpp`/`.cpp`.
* **Tests & Examples:** `test_dataset.cpp`, `test_groupby.cpp`, `test_labels.cpp`, `example_swarmplot.cpp`, `data/input/tips_sample.csv`.

#### Member 4 — ANNA KAGEMULO MTABAZI
* **Modules Written:** `csv_loader.hpp`/`.cpp`, `summary.hpp`/`.cpp`, `annotation.hpp`/`.cpp`.
* **Tests & Examples:** `test_csv_loader.cpp`, `test_summary.cpp`, `test_annotation.cpp`, `example_pointplot.cpp`, `example_annotations.cpp`, `data/input/scores_sample.csv`.

#### Member 5 — RWOTOLARA SUNDAY JUNIOR
* **Modules Written:** `stats.hpp`/`.cpp`, `pointplot.hpp`/`.cpp`.
* **Tests & Examples:** `test_stats.cpp`, `test_pointplot.cpp`, `test_swarmplot_layout.cpp` (independent cross-test), `example_heatmap.cpp`, `data/input/measurements_sample.csv`.

#### Member 6 — KWAGALA REBECCA
* **Modules Written:** `scale.hpp`/`.cpp`, `ticks.hpp`/`.cpp`, `swarmplot.hpp`/`.cpp`.
* **Tests & Examples:** `test_scale.cpp`, `test_ticks.cpp`, `test_swarmplot.cpp`, `test_pointplot_cross.cpp` (independent cross-test), `test_scales_integration.cpp`.

#### Member 7 — AMVIIKO REBECCA NATASHA
* **Modules Written:** `canvas.hpp`/`.cpp`, `draw.hpp`/`.cpp`, `heatmap.hpp`/`.cpp`.
* **Tests & Examples:** `test_canvas.cpp`, `test_draw.cpp`, `test_heatmap.cpp`, `test_figure_integration.cpp`, `test_colorbar_heatmap.cpp`.

#### Member 8 — KALULE WEGNA
* **Modules Written:** `font.hpp`/`.cpp`, `theme.hpp`/`.cpp`, `image_export.hpp`/`.cpp`.
* **Tests & Examples:** `test_font.cpp`, `test_theme.cpp`, `test_image_export.cpp`.

#### Member 9 — KADAMA HUMPHREY ARTHUR
* **Modules Written:** `colormap.hpp`/`.cpp`, `colorbar.hpp`/`.cpp`.
* **Tests & Examples:** `test_colormap.cpp`, `test_colorbar.cpp`, `example_dashboard.cpp`.

#### Member 10 — KALEMA TITUS LARRY
* **Modules Written:** `tests/mini_test.hpp`, `text_util.hpp`/`.cpp`, `preprocess.hpp`/`.cpp`, `legend.hpp`/`.cpp`.
* **Tests & Examples:** `test_mini_test.cpp`, `test_text_util.cpp`, `test_preprocess.cpp`, `test_legend.cpp`, `test_pipeline.cpp`, `test_large_data.cpp` (10,000-row stress test), `example_themes.cpp`.

---

### 11. How We Collaborated as a Team

* **Fixed Weekly Rhythm:** Operated on a strict 3-day implementation schedule:
  * **Mondays:** Author public header contracts (`include/intriviz/*.hpp`) with full function documentation comments.
  * **Wednesdays:** Implement core source logic (`src/*.cpp`) and pass local compilation checks.
  * **Saturdays:** Submit unit tests (`tests/test_*.cpp`), perform peer code reviews, run `ctest`, and merge feature branches into `main`.
* **Git & GitHub Workflow:**
  * Enabled main branch protection requiring pull request reviews before merging.
  * Enforced feature branching (`mN/feature-name`) and conventional commit messages (`feat(...)`, `test(...)`, `fix(...)`).
  * Enforced a strict **"one owner per file per week"** rule, eliminating Git merge conflicts.
* **Code Reviews & Quality Assurance:**
  * Implemented a circular peer review system where Member *N* reviewed Member *N+1* (and Member 10 reviewed Member 1).
  * Conducted independent cross-module testing in Week 4 (e.g., Member 5 wrote independent tests for Member 6's swarm layout; Member 6 tested Member 5's point plot).
  * Executed fresh-machine build verification on clean systems to ensure dependency-free compilation.
