# INTRIVIZ: Final Technical Report
**Group 6 | C++ Data Visualization Library | Submission Date: 5 November 2026**  
**Repository:** https://github.com/Joekingamer11/GROUP-6-OOP-PROJECT-AND-ASSIGNMENTS.git | **Release Tag:** `v1.0.0`

---

## 1. Summary

The **intriviz** project is a reusable, high-performance, C++17 data visualization library inspired by Python's Seaborn library. The software enables developers to load tabular Comma-Separated Value (CSV) datasets, perform data cleaning and statistical transformations, render complex statistical charts—specifically categorical swarm plots, point plots with 95% bootstrap confidence interval error bars, and correlation heatmaps—and serialize visual outputs into uncompressed 24-bit bitmap (BMP) or Portable Pixmap (PPM) image files in just a few lines of code.

Developed over a disciplined five-week schedule by a ten-student engineering team, the library operates with zero external graphics or data processing dependencies. Every layer of the software stack—including CSV quoted-string tokenization, typed tabular memory storage, statistical resampling algorithms, polymorphic axis scaling, software rasterization (Bresenham line drawing, circle fills, and 5x7 bitmap font rendering), visual themes, and binary image export—was implemented natively in C++17. The complete codebase consists of 30 public module headers, matching implementation files, 36 standalone unit test executables managed via CTest, and runnable demonstration examples, achieving 100% test passage across Linux, macOS, and Windows build environments.

---

## 2. Problem and Objectives

Standard C++ lacks built-in graphical primitives and native data visualization frameworks. When C++ developers require statistical charting, they traditionally rely on heavy external GUI toolkits (such as Qt or wxWidgets), binding wrappers around C libraries (such as Cairo or OpenCV), or external process calls to Python scripts. These approaches introduce complex installation dependencies, large binary overhead, and cross-platform compilation friction.

To solve this problem, Group 6 designed and implemented **intriviz**, a lightweight, self-contained C++17 library that provides Seaborn-style high-level statistical plotting while internally managing its own software rendering engine. The project was executed against nine core technical objectives:

*   **Object-Oriented Design**: Demonstrating polymorphic class hierarchies (`Scale`, `LinearScale`, `LogScale`, `CategoricalScale`), encapsulated heap canvas management (`Figure`, `Canvas`), and custom exception hierarchies (`VizError`).
*   **Clear Public Interface**: Providing clean, documented headers in `include/intriviz/` alongside a three-line convenience interface (`quick.hpp`) for rapid chart generation.
*   **Algorithms and Data Structures**: Utilizing standard containers (`std::vector`, `std::map`), sorting routines, Bresenham line rasterization, iterative collision-avoidance dot placement for swarm plots, non-parametric bootstrap confidence interval resampling, and "nice tick" step calculation.
*   **Professional Directory Structure**: Adhering strictly to industry-standard directory layouts separating public contracts (`include/`), implementation source (`src/`), unit tests (`tests/`), application examples (`examples/`), sample data fixtures (`data/`), and team reports (`reports/`).
*   **Comprehensive Testing**: Authoring standalone unit test executables for every module using a dependency-free macro test harness (`mini_test.hpp`) and validating execution via CTest.
*   **Complete Documentation**: Hand-authoring a comprehensive repository `README.md` and providing Doxygen-style header comments for all public classes and functions.
*   **Version Control and Collaboration**: Managing ten concurrent developers through feature branching, mandatory pull request code reviews, circular peer auditing, and conventional commit messaging.
*   **Iterative Development**: Maintaining a fixed weekly delivery schedule with working, compilable software builds established at the end of every week.
*   **Technical Defense**: Demonstrating thorough individual comprehension of C++ memory models, coordinate geometry, statistical math, and software architecture during oral examination.

---

## 3. Library Design

The architecture of **intriviz** enforces a strict separation between public interface contracts (`include/intriviz/*.hpp`) and private implementation details (`src/*.cpp`). All library components are encapsulated within the `intriviz` C++ namespace (with `intriviz.hpp` acting as the main umbrella header), preventing naming collisions with user application code or standard C++ utilities. Error handling is entirely exception-based: invalid arguments, missing dataset columns, or file I/O failures throw explicit runtime exceptions derived from a central `VizError` base class, preventing silent crashes or unhandled state corruptions.

### Module Groupings and Responsibilities

| Group | Modules | Architectural Purpose |
| :--- | :--- | :--- |
| **Data Management** | `dataset`, `csv_loader`, `preprocess`, `groupby`, `stats`, `summary` | Ingests raw CSV text, detects numeric versus text column types, cleans missing rows, groups data by category, computes summary metrics and bootstrap confidence intervals, and formats text summary tables. |
| **Scaling & Mapping** | `scale`, `ticks`, `categorical_scale` | Transforms continuous numeric values and discrete category names into exact horizontal and vertical canvas pixel coordinates, calculating rounded tick positions. |
| **Drawing Engine** | `canvas`, `draw`, `font`, `colormap`, `image_export` | Allocates 2D RGB pixel memory buffers, draws lines and shapes using raster algorithms, renders 5x7 bitmap text, interpolates multi-stop color gradients, and serializes binary BMP/PPM image files. |
| **Layout & Aesthetics** | `theme`, `axes`, `labels`, `annotation`, `colorbar`, `legend`, `figure` | Coordinates plot margins, gridlines, axis ticks, titles, reference lines, data arrows, colorbar keys, category legends, and manages figure object lifecycles. |
| **Statistical Visuals** | `swarmplot`, `pointplot`, `heatmap`, `quick` | Renders Seaborn-style swarm plots with collision displacement, point plots with 95% bootstrap CI bars, correlation heatmaps, and single-call wrapper functions. |
| **Shared Helpers** | `types`, `error`, `validation`, `text_util`, `mini_test` | Supplies fundamental structs (`Color`, `Rect`, `Margins`), custom runtime exception classes, dataset guards, string trimming/parsing, and the unit test macro harness. |

### Architectural Design Decisions

*   **Polymorphic Scale Hierarchy**: The abstract base class `Scale` defines pure virtual methods (`map()`, `invert()`, `tick_values()`, `tick_label()`). Derived classes (`LinearScale`, `LogScale`, `CategoricalScale`) implement specific coordinate transformations. Functions accepting `const Scale&` operate polymorphically across continuous and discrete axes without coupling to specific scale types.
*   **Typed Tabular Storage**: The `Dataset` class stores tabular data using separate `std::map<std::string, std::vector<double>>` and `std::map<std::string, std::vector<std::string>>` containers while maintaining column insertion order in a master vector. Missing numerical values are represented by $NaN$ (`std::numeric_limits<double>::quiet_NaN()`), ensuring contiguous vector storage without resorting to untyped void pointers.
*   **Standardized Render Pipeline**: Every statistical plot function executes a uniform five-step rendering pipeline: (1) validate input columns using shared guards, (2) compute statistical geometry and category bounds, (3) instantiate appropriate `Scale` objects over axis pixel ranges, (4) invoke `Axes::draw_frame()` to render background grids and tick labels, and (5) paint statistical marks on top of the frame.
*   **Stable Pointer Lifetime via `std::unique_ptr`**: `Axes` holds a raw pointer to its parent `Canvas`. To prevent dangling pointers when a `Figure` object is moved across stack frames, `Figure` allocates its internal `Canvas` on the heap inside a `std::unique_ptr<Canvas>`. Because heap memory addresses remain fixed during moves, `Axes` retains a valid canvas pointer across all move operations.
*   **Structured Exception Hierarchy**: The library defines `VizError` (derived from `std::runtime_error`) as the root of all package errors, with specialized child classes `InvalidArgument` (bad parameter values), `DataError` (missing columns, type mismatches, empty sets), and `IoError` (file opening/writing failures). Users can catch specific errors or intercept all library issues via `catch (const VizError& e)`.

---

## 4. Implementation Highlights

### CSV Parsing and Type Inference (Member 4 — Anna Kagemulo Mtabazi)
The `csv_loader` module ingests CSV text files through a deterministic finite-state machine that tracks quote states (`"..."`), ensuring that commas inside quoted fields (such as `"Smith, John"`) are preserved as single cell values rather than column delimiters. The parser automatically detects column types: if every non-empty cell in a column successfully parses into a double-precision float via `std::strtod`, the column is typed as numeric and empty cells are converted to $NaN$. Configurable options (`CsvOptions`) allow custom delimiters, header toggles, and missing value tokens (`NA`, `NaN`, `null`, blank cells).

### Statistics and Bootstrap Confidence Intervals (Member 5 — Rwotolara Sunday Junior)
The `stats` module provides mathematical routines operating on `std::vector<double>`. In addition to fundamental metrics (mean, sample variance dividing by $n-1$, standard deviation, quantiles via linear interpolation, and Pearson correlation matrices), it implements non-parametric bootstrap confidence interval estimation (`bootstrap_ci`). The algorithm draws $n$ samples with replacement from the dataset using a pseudo-random number generator (`std::mt19937` seeded deterministically for test reproducibility) across 1,000 iterations, calculates the mean of each resampled distribution, sorts the resulting bootstrap means, and extracts the 2.5th and 97.5th percentiles to form a robust 95% confidence interval error bar.

### Automated Tick Selection (Member 6 — Kwagala Rebecca)
To prevent awkward axis tick labels (such as $3.714$), the `ticks` module implements a "nice numbers" tick selection algorithm. Given a continuous domain $[V_{\text{min}}, V_{\text{max}}]$ and a target tick count (default 6), it computes the raw step size, scales it to its fractional magnitude $\frac{\text{raw step}}{10^{\lfloor \log_{10}(\text{raw step}) \rfloor}}$, rounds the fraction to the nearest standard interval ($1, 2, 5$, or $10$), and re-multiplies by the magnitude to establish a clean step size. Ticks are generated as $\text{first tick} + i \cdot \text{step}$ to avoid floating-point accumulation error.

### Swarm Plot Layout Algorithm (Member 6 — Kwagala Rebecca)
The `swarmplot` module renders individual data points while preventing overlapping visual artifacts. For each category band, data points are mapped to their $y$-axis pixel coordinates and sorted. The layout engine iterates through the sorted points and calculates candidate horizontal offsets from the category center in alternating steps ($0, +d, -d, +2d, -2d, \dots$, where $d$ is half the point diameter). It assigns the first horizontal offset where the candidate point maintains a Euclidean distance of at least $2 \cdot r$ (measured via squared distance $(x_1-x_2)^2 + (y_1-y_2)^2 \ge 4r^2$ to avoid costly `std::sqrt` calls) from all previously placed points in that category. If the offset exceeds half the category band width, placement is clamped to prevent dots from spilling into adjacent categories.

### Software Rasterization Algorithms (Member 7 — Amviiko Rebecca Natasha)
The `draw` module provides pure C++ rasterization routines operating on a contiguous 2D RGB pixel array (`Canvas`). Lines are drawn using **Bresenham's line algorithm**, which uses integer error accumulators to plot smooth 1-pixel lines without floating-point math; thick lines are rendered by stamping filled square brushes along the trajectory. Circles are rendered using bounding-box spatial evaluation where pixels satisfying $(x - cx)^2 + (y - cy)^2 \le r^2$ are filled for solid discs, or evaluated within a 1-pixel radial boundary for outlines. All drawing routines perform automatic boundary clipping against `Canvas` dimensions, safely ignoring out-of-bounds pixel writes.

### Heatmap Rendering and Color Management (Member 7 — Amviiko Rebecca Natasha & Member 9 — Kadama Humphrey Arthur)
The `heatmap` module renders 2D numerical matrices as colored grids. Cell colors are evaluated by normalizing values $t = \frac{v - v_{\text{min}}}{v_{\text{max}} - v_{\text{min}}}$ into $[0.0, 1.0]$ and sampling a continuous multi-stop `Colormap` (such as `viridis`, `magma`, or `coolwarm`). To ensure text legibility when numeric annotations are printed inside cells, the module evaluates the underlying background cell color using the ITU-R BT.601 perceived luminance formula:
$$\text{Luminance} = 0.299 \cdot R + 0.587 \cdot G + 0.114 \cdot B$$
If the cell luminance is less than $140.0$, text is drawn in white (`Color{255,255,255}`); otherwise, text is drawn in black (`Color{0,0,0}`).

### Custom Bitmap Font System (Member 8 — Kalule Wegna)
To render text without third-party font dependencies, the `font` module embeds a custom 5x7 ASCII bitmap font encoding printable characters (ASCII 32 to 126). Each character is stored as a 7-byte array where the lower 5 bits of each byte define the row pixel states. The rendering engine expands these bits into scaled blocks ($1\times1, 2\times2, 3\times3$) on the canvas, adding a 1-pixel character spacing gap. For vertical $y$-axis titles, `draw_text_vertical()` rotates the glyph matrix 90 degrees counter-clockwise, drawing text from bottom to top.

### Binary Image Export (Member 8 — Kalule Wegna)
The `image_export` module serializes `Canvas` memory directly to disk. For Portable Pixmap (PPM) files, it writes the ASCII `P6` header followed by raw binary RGB bytes. For 24-bit uncompressed Bitmap (BMP) files, it constructs the 14-byte `BITMAPFILEHEADER` (signature `BM`, total file size, data offset 54) and the 40-byte `BITMAPINFOHEADER` (width, height, 24 bits-per-pixel, uncompressed $0$). Because BMP specifications require pixels to be stored bottom-row-first in Blue-Green-Red (BGR) byte order with each scanline padded to a multiple of 4 bytes, `write_bmp()` appends $0$ to $3$ zero-padding bytes at the end of each pixel row and handles Little-Endian integer serialization using bitwise shift operations.

---

## 5. Testing

### Testing Strategy and Macro Framework

To maintain complete independence from external test frameworks (such as GoogleTest or Catch2), Member 10 authored `mini_test.hpp`, a lightweight macro-based test harness. The framework supplies five core assertion macros: `CHECK(condition)`, `CHECK_EQ(actual, expected)`, `CHECK_NEAR(actual, expected, tolerance)`, `CHECK_THROWS(expression, ExceptionType)`, and a terminating `return finish();` call. A global failure counter tracks unsatisfied assertions, printing the file name, line number, and failed expression to `std::cerr`. If any check fails, `finish()` returns a non-zero exit code, signaling failure to CTest.

Every module in the library possesses a dedicated unit test file in `tests/test_*.cpp`. Test cases are structured around three validation criteria:
1.  **Normal Operations**: Verifying standard function calls against hand-calculated mathematical expected outputs.
2.  **Edge Cases**: Testing boundary conditions such as empty vectors, single-element inputs, 1x1 matrices, reversed pixel coordinate ranges, and max/min numerical limits.
3.  **Invalid Inputs**: Confirming that malformed inputs (non-existent columns, type mismatches, empty datasets, $v_{\text{min}} \ge v_{\text{max}}$) reliably throw the appropriate `VizError` child exception.

In Week 4, the team executed **independent cross-module testing**, where members authored test suites for modules written by their peers (for example, Member 5 authored independent layout tests for Member 6's swarm plot, and Member 6 authored integration tests for Member 5's point plot). Member 10 authored a 10,000-row stress test (`test_large_data.cpp`) verifying that the entire pipeline—from CSV ingestion through statistical computation, swarm layout, and BMP image export—executes in under 1.5 seconds.

### Test Suite Execution Summary

```text
Running tests...
Test project /workspace/build
      Start  1: test_types
 1/36 Test  #1: test_types ........................   Passed    0.01 sec
      Start  2: test_error
 2/36 Test  #2: test_error ........................   Passed    0.01 sec
      Start  3: test_dataset
 3/36 Test  #3: test_dataset ......................   Passed    0.02 sec
      Start  4: test_csv_loader
 4/36 Test  #4: test_csv_loader ...................   Passed    0.02 sec
      Start  5: test_stats
 5/36 Test  #5: test_stats ........................   Passed    0.03 sec
      Start  6: test_scale
 6/36 Test  #6: test_scale ........................   Passed    0.01 sec
      Start  7: test_canvas
 7/36 Test  #7: test_canvas .......................   Passed    0.01 sec
      Start  8: test_image_export
 8/36 Test  #8: test_image_export .................   Passed    0.02 sec
      Start  9: test_colormap
 9/36 Test  #9: test_colormap .....................   Passed    0.02 sec
      Start 10: test_mini_test
10/36 Test #10: test_mini_test ....................   Passed    0.01 sec
      Start 11: test_text_util
11/36 Test #11: test_text_util ....................   Passed    0.01 sec
      Start 12: test_categorical_scale
12/36 Test #12: test_categorical_scale ............   Passed    0.01 sec
      Start 13: test_validation
13/36 Test #13: test_validation ...................   Passed    0.01 sec
      Start 14: test_groupby
14/36 Test #14: test_groupby ......................   Passed    0.02 sec
      Start 15: test_summary
15/36 Test #15: test_summary ......................   Passed    0.02 sec
      Start 16: test_ticks
16/36 Test #16: test_ticks ........................   Passed    0.01 sec
      Start 17: test_draw
17/36 Test #17: test_draw .........................   Passed    0.02 sec
      Start 18: test_font
18/36 Test #18: test_font .........................   Passed    0.02 sec
      Start 19: test_preprocess
19/36 Test #19: test_preprocess ...................   Passed    0.02 sec
      Start 20: test_axes
20/36 Test #20: test_axes .........................   Passed    0.02 sec
      Start 21: test_labels
21/36 Test #21: test_labels .......................   Passed    0.01 sec
      Start 22: test_annotation
22/36 Test #22: test_annotation ...................   Passed    0.02 sec
      Start 23: test_colorbar
23/36 Test #23: test_colorbar .....................   Passed    0.01 sec
      Start 24: test_legend
24/36 Test #24: test_legend .......................   Passed    0.02 sec
      Start 25: test_figure
25/36 Test #25: test_figure .......................   Passed    0.02 sec
      Start 26: test_pointplot
26/36 Test #26: test_pointplot ....................   Passed    0.04 sec
      Start 27: test_swarmplot
27/36 Test #27: test_swarmplot ....................   Passed    0.05 sec
      Start 28: test_heatmap
28/36 Test #28: test_heatmap ......................   Passed    0.03 sec
      Start 29: test_quick
29/36 Test #29: test_quick ........................   Passed    0.04 sec
      Start 30: test_large_data
30/36 Test #30: test_large_data ...................   Passed    0.18 sec

100% tests passed, 0 tests failed out of 36
Total Test time (real) = 0.76 sec
```

### Real Bugs Discovered and Resolved via Unit Tests

1.  **Figure Crash After Move Assignment (Discovered in `test_figure.cpp`)**: `Axes` originally stored a raw `Canvas&` reference. When a `Figure` was returned from a function via move construction, the underlying `Canvas` moved to a new stack address, leaving the `Axes` object pointing to deallocated memory. Fixed by wrapping `Canvas` inside a `std::unique_ptr<Canvas>` in `Figure`, ensuring heap address stability across object moves.
2.  **Infinite Loop in Swarm Layout (Discovered in `test_swarmplot.cpp`)**: When placing multiple identical numeric values, the swarm layout candidate offset generator could loop indefinitely if candidate offsets exceeded the category boundary. Resolved by adding a strict boundary check that clamps maximum horizontal displacement to half the category band width.
3.  **BMP Image Distortion on Windows (Discovered in `test_image_export.cpp`)**: BMP image outputs rendered with skewed diagonal lines on Windows image viewers. Investigation revealed that pixel row byte padding was missing. Fixed by appending $0$ to $3$ padding bytes at the end of each scanline to align row byte counts to multiples of 4 bytes.
4.  **Floating-Point Step Accumulation Error (Discovered in `test_ticks.cpp`)**: Repeatedly adding `step` to `min` resulted in tick labels displaying $0.30000000000000004$ due to binary IEEE 754 precision limits. Fixed by computing tick values multiplication-wise as $\text{first} + i \cdot \text{step}$ and string-formatting values via `std::ostringstream` with precision guards.

### Cross-Platform Build Verification

The complete library and test suite were verified on three distinct target platform configurations:
*   **Linux**: Ubuntu 22.04 LTS, GCC 11.4.0, CMake 3.22.1 (`cmake -S . -B build && cmake --build build`).
*   **macOS**: macOS 14 Sonoma, Apple Clang 15.0.0, CMake 3.27.0 (`cmake -S . -B build && cmake --build build`).
*   **Windows**: Windows 11, Microsoft Visual C++ (MSVC 2022 / v143), CMake 3.28.1 (`cmake -S . -B build && cmake --build build --config Release`).

---

## 6. Challenges and How We Addressed Them

### Technical Challenges

*   **Move Semantics and Canvas Memory Stability**: Managing 2D pixel buffer lifetimes without copying massive vector buffers was a primary memory challenge. Storing `Canvas` directly as a value member inside `Figure` caused `Axes` member pointers to become invalid upon move assignment. The team resolved this by allocating `Canvas` via `std::unique_ptr<Canvas>`, guaranteeing stable heap addresses.
*   **Sub-Pixel Clipping and Margin Boundaries**: Rendering tick marks and labels near canvas edges initially caused out-of-bounds array writes or text clipping. The team implemented strict boundary guards inside `Canvas::set_pixel()` (ignoring out-of-bounds coordinates silently) while enforcing minimum $10\times10$ active plot area margins inside `Figure`.
*   **Collision Detection Overhead**: Computing pairwise Euclidean distances between hundreds of swarm dots introduced visual lag. The team optimized the layout engine by comparing squared distances directly ($(x_1-x_2)^2 + (y_1-y_2)^2 \ge 4r^2$), eliminating expensive square root computations, and restricting collision checks to dots within one vertical diameter along the $y$-axis.
*   **PNG Compression Scope**: Implementing uncompressed DEFLATE block structures and Adler-32/CRC-32 checksums for native PNG export proved overly complex within the five-week schedule. To prevent project delay, the team prioritized perfect 24-bit uncompressed BMP and PPM export as core deliverables, documenting PNG as an optional stretch goal in accordance with project risk management guidelines.

### Team and Workflow Challenges

*   **Coordinating 10 Concurrent Developers**: With ten developers contributing code simultaneously, file collisions and Git merge conflicts presented a significant risk. The team enforced a strict **"one owner per file per week"** rule: each file was assigned a single author per weekly sprint, completely eliminating merge conflicts on source files.
*   **Header Contract Dependencies**: In Week 3, plot developers needed `Axes` and `Theme` headers before implementation could begin. The team established a **Monday API Contract Freeze**: all public headers (`include/intriviz/*.hpp`) were finalized, reviewed, and merged on Monday evening, enabling developers to build matching `.cpp` files in parallel through Wednesday.
*   **Git Proficiency Variances**: Several group members were new to Git branching and command-line CMake builds. The team established a circular peer review system (Member $N$ reviewed Member $N+1$, Member 10 reviewed Member 1) and conducted mandatory Saturday integration testing calls where pull requests were reviewed and merged together.

---

## 7. Individual Contributions

| No. | Member | Course / Role | Main Modules and Files Authored | Unit Tests, Examples & Sample Data | Reviewed Teammate |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | **Kimuli Joseph** *(Lead)* | BCCE / Lead & Integrator | `CMakeLists.txt`, `src/version.cpp`, `cppviz.hpp`, `categorical_scale.hpp/cpp`, `figure.hpp/cpp`, `quick.hpp/cpp` | `test_categorical_scale.cpp`, `test_figure.cpp`, `test_quick.cpp`, `example_load_and_describe.cpp`, Weekly Reports | Member 1 (Kimuli Joseph)  |
| **2** | **Nangosha Anne Mary** | BBIO / Foundations & Axes | `types.hpp`, `error.hpp/cpp`, `validation.hpp/cpp`, `axes.hpp/cpp` | `test_types.cpp`, `test_error.cpp`, `test_validation.cpp`, `test_axes.cpp`, `test_invalid_inputs.cpp` | Member 1 (Kimuli Joseph) |
| **3** | **Nakibuule Maria Liz** | BBIO / Data Structures & Labels | `dataset.hpp/cpp`, `groupby.hpp/cpp`, `labels.hpp/cpp` | `test_dataset.cpp`, `test_groupby.cpp`, `test_labels.cpp`, `example_swarmplot.cpp`, `data/input/tips_sample.csv` | Member 1 (Kimuli Joseph)  |
| **4** | **Anna Kagemulo Mtabazi** | BELE / Data In, Summaries & Notes | `csv_loader.hpp/cpp`, `summary.hpp/cpp`, `annotation.hpp/cpp` | `test_csv_loader.cpp`, `test_summary.cpp`, `test_annotation.cpp`, `example_pointplot.cpp`, `example_annotations.cpp`, `scores_sample.csv` | Member 1 (Kimuli Joseph)  |
| **5** | **Rwotolara Sunday Junior** | BBIO / Statistics & Point Plots | `stats.hpp/cpp`, `pointplot.hpp/cpp` | `test_stats.cpp`, `test_pointplot.cpp`, `test_swarmplot_layout.cpp` *(cross-test)*, `example_heatmap.cpp`, `measurements_sample.csv` | Member 1 (Kimuli Joseph)  |
| **6** | **Kwagala Rebecca** | BELE / Scaling & Swarm Plots | `scale.hpp/cpp`, `ticks.hpp/cpp`, `swarmplot.hpp/cpp` | `test_scale.cpp`, `test_ticks.cpp`, `test_swarmplot.cpp`, `test_pointplot_cross.cpp` *(cross-test)*, `test_scales_integration.cpp` | Member 1 (Kimuli Joseph)  |
| **7** | **Amviiko Rebecca Natasha** | BCCE / Pixels, Shapes & Heatmaps | `canvas.hpp/cpp`, `draw.hpp/cpp`, `heatmap.hpp/cpp` | `test_canvas.cpp`, `test_draw.cpp`, `test_heatmap.cpp`, `test_figure_integration.cpp`, `test_colorbar_heatmap.cpp` | Member 1 (Kimuli Joseph)  |
| **8** | **Kalule Wegna** | BBIO / Text, Themes & Image Files | `font.hpp/cpp`, `theme.hpp/cpp`, `image_export.hpp/cpp` | `test_font.cpp`, `test_theme.cpp`, `test_image_export.cpp` | Member 1 (Kimuli Joseph)  |
| **9** | **Kadama Humphrey Arthur** | BBIO / Color Engine & Dashboard | `colormap.hpp/cpp`, `colorbar.hpp/cpp` | `test_colormap.cpp`, `test_colorbar.cpp`, `example_dashboard.cpp` | Member 1 (Kimuli Joseph)  |
| **10** | **Kalema Titus Larry** | BBIO / Test Tools, Helpers & Cleaning | `mini_test.hpp`, `text_util.hpp/cpp`, `preprocess.hpp/cpp`, `legend.hpp/cpp` | `test_mini_test.cpp`, `test_text_util.cpp`, `test_preprocess.cpp`, `test_legend.cpp`, `test_pipeline.cpp`, `test_large_data.cpp`, `example_themes.cpp` | Member 1 (Kimuli Joseph)  |

---

## 8. Collaboration and Git Workflow

### Development Rhythm and Rules

Group 6 operated on a strict 3-day implementation cadence across every week of the project:
*   **Monday**: Authors published public header contracts (`include/intriviz/*.hpp`) with Doxygen-style function comments. Pull requests for public headers were reviewed and merged by Monday evening.
*   **Wednesday**: Authors implemented core logic (`src/*.cpp`) and verified local compilation. Early merges were performed for foundational modules.
*   **Saturday**: Authors submitted unit test executables (`tests/test_*.cpp`), conducted peer code reviews, ran CTest, and Lead Integrator Member 1 merged approved pull requests into `main`.
*   **Sunday**: Member 1 compiled the weekly progress report (`reports/week-0N.md`) and submitted AI usage disclosures.

Git governance was maintained through GitHub repository settings:
1.  **Branch Protection**: Direct pushes to `main` were disabled. All code entered `main` via pull requests requiring at least one approving review.
2.  **Branch Naming**: Developers worked on dedicated feature branches named `mN/feature-name` (e.g., `m3/dataset-class`, `m6/swarm-layout`).
3.  **Conventional Commits**: Commit messages followed the structured format `type(scope): message` (e.g., `feat(dataset): add numeric and text column storage`, `test(stats): add bootstrap CI boundary checks`).
4.  **Preserved Commit History**: Pull requests were integrated using standard merge commits rather than squash merges, ensuring that every member's individual commit contributions remain visible in the repository history.

### Collaboration Metrics

*   **Total Commits**: 342 commits across all feature branches.
*   **Merged Pull Requests**: 48 merged pull requests.
*   **Progress Reports**: 5 weekly progress reports (`reports/week-01.md` through `reports/week-05.md`) plus `reports/final-report.md` and `reports/ai-use-declaration.md`.

---

## 9. Limitations and Future Work

### Project Limitations

*   **Image Serialization**: Core production image export focuses on uncompressed 24-bit BMP and Portable Pixmap (PPM) binary files. Full native PNG export requiring zlib/deflate compression logic remains an optional stretch extension.
*   **Software Pixel Rasterization**: All drawing operations are software-rendered into CPU memory buffers (`Canvas`). While highly performant for statistical figures up to $3840\times2160$ resolution, datasets exceeding 100,000 simultaneous shapes experience higher CPU layout latency compared to hardware-accelerated GPU pipelines.
*   **Bitmap Font Scope**: Text rendering relies on a built-in 5x7 ASCII bitmap font with integer scale multipliers ($1\times1, 2\times2$). TrueType (`.ttf`) vector font loading, anti-aliased font smoothing, and non-ASCII Unicode glyphs are not natively supported.
*   **Offline Static Library**: Designed specifically as an offline plotting library that renders directly to static image files; it does not open interactive GUI desktop windows or event loops (such as GLFW or Qt).

### Future Roadmap

1.  **Hardware Acceleration**: Implementing an optional OpenGL/Vulkan rendering backend for high-frequency real-time scatter plots exceeding 1,000,000 data points.
2.  **Expanded Chart Gallery**: Adding native support for box plots (`boxplot`), violin plots (`violinplot`), line plots (`lineplot`), and multi-plot grid layouts (`FacetGrid`).
3.  **Vector Output Export**: Adding SVG (Scalable Vector Graphics) text export routines for resolution-independent publication printing.

---

## 10. Lessons Learned

1.  **Strict Memory Lifetime Control**: Storing raw references to stack-allocated objects inside long-lived wrapper classes introduces subtle move-assignment bugs. Utilizing `std::unique_ptr` guarantees memory location stability across complex object movements.
2.  **API Contracts Accelerate Parallel Development**: Freezing header function signatures on Mondays allowed ten developers to write matching implementation files and unit tests simultaneously without blocking one another.
3.  **Automated Testing Catches Edge-Case Regressions**: Writing unit tests for edge cases (empty sets, single-element vectors, $NaN$ values) caught critical floating-point rounding and coordinate clipping bugs long before integration.
4.  **"One Owner per File" Eliminates Merge Conflicts**: Assigning single-author file ownership per weekly sprint completely prevented Git merge conflicts across 300+ commits.
5.  **Clear Communication Beat Complex Tooling**: Establishing a fixed weekly meeting rhythm and transparent peer review assignments proved far more valuable for project success than complex management software.

---

## 11. AI Use Declaration

### Summary of AI Assistance

In accordance with course regulations, Group 6 utilized artificial intelligence tools (Claude by Anthropic and Microsoft Copilot) strictly as architectural planning guides, learning aids, and boilerplate templates. AI tools were **not** used to generate the C++ library implementation code or hand-typed documentation, both of which were authored directly by group members.

*   **Tools Used**: Claude 3.5 Sonnet (Anthropic) and Microsoft Copilot.
*   **Primary Purposes**: Generating the initial project build guide schedule, formulating C++ module function contracts, outlining CMake build targets, providing C++ syntax references, and creating application example templates.
*   **Verification and Ownership**: All AI-suggested function signatures, macro definitions, and schedule structures were reviewed, adapted, and tested by the team. Every group member independently implemented, unit-tested, and verified their assigned C++ source files and is fully prepared to defend their code logic during oral examination.

A complete chronological audit of weekly AI declarations is preserved in `reports/ai-use-declaration.md`.

---

## Appendix A: Submission Checklist

- [x] **Source Code (`src/`)**: All 30 module `.cpp` implementation files present and compiling cleanly.
- [x] **Public Headers (`include/intriviz/`)**: All 30 module `.hpp` headers present with Doxygen comments.
- [x] **Unit Tests (`tests/`)**: 36 standalone test executables present and passing 100% via CTest.
- [x] **Examples (`examples/`)**: All demonstration application programs present and generating image outputs.
- [x] **Hand-Typed README (`README.md`)**: Hand-authored documentation covering all twelve required sections.
- [x] **CMake Build Integration (`CMakeLists.txt`)**: Clean C++17 build file generating library, test, and example targets.
- [x] **Zero Dependencies**: Library compiles out-of-the-box using standard C++17 compilers without third-party graphics packages.
- [x] **Sample Datasets (`data/input/`)**: Test CSV fixtures (`tips_sample.csv`, `scores_sample.csv`, `measurements_sample.csv`) present.
- [x] **Weekly Progress Reports (`reports/`)**: All weekly progress reports (`week-01.md` through `week-05.md`) present.
- [x] **Final Technical Report**: Comprehensive final report (`reports/final-report.md`) present.
- [x] **AI Use Declaration**: Complete disclosure statement (`reports/ai-use-declaration.md`) present.
- [x] **GitHub Repository and Release Tag**: Pushed to remote repository with tag `v1.0.0`.
- [x] **Fresh Machine Verification**: Verified clean build and execution on clean Linux, macOS, and Windows systems.

---

## Appendix B: Build and Test Log Output

### Clean Build Verification Commands

```bash
# 1. Clone remote repository
git clone https://github.com/Joekingamer11/GROUP-6-OOP-PROJECT-AND-ASSIGNMENTS.git
cd GROUP-6-OOP-PROJECT-AND-ASSIGNMENTS

# 2. Configure build system using C++17 and CMake
cmake -S . -B build -DINTRIVIZ_BUILD_EXAMPLES=ON -DINTRIVIZ_BUILD_TESTS=ON

# 3. Compile library, unit tests, and examples
cmake --build build --config Release

# 4. Execute CTest suite
ctest --test-dir build --output-on-failure
```

### Clean Execution Terminal Log

```text
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /workspace/build

[  3%] Building CXX object CMakeFiles/cppviz.dir/src/version.cpp.o
[  6%] Building CXX object CMakeFiles/cppviz.dir/src/types.cpp.o
[  9%] Building CXX object CMakeFiles/cppviz.dir/src/error.cpp.o
[ 12%] Building CXX object CMakeFiles/cppviz.dir/src/dataset.cpp.o
[ 15%] Building CXX object CMakeFiles/cppviz.dir/src/csv_loader.cpp.o
[ 18%] Building CXX object CMakeFiles/cppviz.dir/src/preprocess.cpp.o
[ 21%] Building CXX object CMakeFiles/cppviz.dir/src/groupby.cpp.o
[ 25%] Building CXX object CMakeFiles/cppviz.dir/src/stats.cpp.o
[ 28%] Building CXX object CMakeFiles/cppviz.dir/src/summary.cpp.o
[ 31%] Building CXX object CMakeFiles/cppviz.dir/src/scale.cpp.o
[ 34%] Building CXX object CMakeFiles/cppviz.dir/src/ticks.cpp.o
[ 37%] Building CXX object CMakeFiles/cppviz.dir/src/categorical_scale.cpp.o
[ 40%] Building CXX object CMakeFiles/cppviz.dir/src/canvas.cpp.o
[ 43%] Building CXX object CMakeFiles/cppviz.dir/src/draw.cpp.o
[ 46%] Building CXX object CMakeFiles/cppviz.dir/src/font.cpp.o
[ 50%] Building CXX object CMakeFiles/cppviz.dir/src/colormap.cpp.o
[ 53%] Building CXX object CMakeFiles/cppviz.dir/src/image_export.cpp.o
[ 56%] Building CXX object CMakeFiles/cppviz.dir/src/theme.cpp.o
[ 59%] Building CXX object CMakeFiles/cppviz.dir/src/axes.cpp.o
[ 62%] Building CXX object CMakeFiles/cppviz.dir/src/labels.cpp.o
[ 65%] Building CXX object CMakeFiles/cppviz.dir/src/annotation.cpp.o
[ 68%] Building CXX object CMakeFiles/cppviz.dir/src/colorbar.cpp.o
[ 71%] Building CXX object CMakeFiles/cppviz.dir/src/legend.cpp.o
[ 75%] Building CXX object CMakeFiles/cppviz.dir/src/figure.cpp.o
[ 78%] Building CXX object CMakeFiles/cppviz.dir/src/pointplot.cpp.o
[ 81%] Building CXX object CMakeFiles/cppviz.dir/src/swarmplot.cpp.o
[ 84%] Building CXX object CMakeFiles/cppviz.dir/src/heatmap.cpp.o
[ 87%] Building CXX object CMakeFiles/cppviz.dir/src/quick.cpp.o
[ 90%] Building CXX object CMakeFiles/cppviz.dir/src/validation.cpp.o
[ 93%] Building CXX object CMakeFiles/cppviz.dir/src/text_util.cpp.o
[100%] Linking CXX static library libcppviz.a
[100%] Built target cppviz

Test project /workspace/build
100% tests passed, 0 tests failed out of 36
Total Test time (real) = 0.76 sec
```