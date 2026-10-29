# AI Use Declaration — Group 6 (cppviz C++ Data Visualization Library)

## 1. Executive Policy Statement & Compliance Framework

This document constitutes the official AI Use Declaration for Group 6 in accordance with Section 10 of the Course Instructions for the C++ Data Visualization Library project (`intriviz`). The project objective required the collaborative design and implementation of a dependency-free, C++17 statistical visualization library inspired by Python's Seaborn.

The development team strictly adhered to the course directive regarding artificial intelligence: AI tools were employed solely as educational aids, architectural planning assistants, and diagnostic helpers. At no point was the core library codebase, rendering engine, or technical documentation generated automatically by AI. Every public header (`include/intriviz/*.hpp`), implementation file (`src/*.cpp`), test executable (`tests/test_*.cpp`), and user documentation artifact (`README.md`) was hand-typed, tested, and reviewed by the designated member owner.

The team established a formal AI governance framework during Week 0 to ensure transparency and maintain academic integrity across all ten team members:

*   **Permitted AI Usage**: Architectural planning, clarification of C++17 language standards, syntax explanation for complex compiler error diagnostics, template drafting for project management reports, and peer-review checklist generation.
*   **Prohibited AI Usage**: Automated code generation of C++ implementation files, copy-pasted README documentation, automated unit test generation, and delegating problem-solving without individual code comprehension.
*   **Mandatory Verification**: Every AI-assisted diagnostic or explanation was independently verified through manual C++ implementation, local CMake build compilation, and CTest execution.

---

## 2. Chronological Weekly AI Usage Audit

In compliance with weekly progress reporting guidelines, AI usage was recorded at the close of each development cycle. The chronology below outlines the specific tools utilized, the scope of assistance, the underlying technical rationale, and the verification methods employed.

### 2.1 Week 0 & Week 1: Architecture, Contracts, and Initial Planning

During the initial project kickoff and foundational setup (2 October – 11 October 2026), AI tools were utilized exclusively to establish project governance and file allocation contracts for the ten-person team.

To prevent merge conflicts among ten concurrent developers, the team required a rigid architectural contract defining 30 public headers, data structures, and function signatures. Claude (Anthropic) was consulted to assist in structuring the initial build guide template, file allocation matrix, and git workflow rules.

*   **Tool Utilized**: Claude (Anthropic).
*   **Purpose**: Assistance in drafting the structural framework of `INTRIVIZ-group6-build-guide.docx` (The document the group used to assign tasks and determine weekly workflow) and formulating the single-owner-per-file directory layout.
*   **Reason**: As beginner C++ developers undertaking a large-scale collaborative project, the team required a clear structural contract so that members could code independently without header name collisions or file access deadlocks.
*   **Verification**: The team reviewed the proposed header contracts and build guide during a meeting on 4 October 2026, manually adjusting file ownership assignments, function names, and exception hierarchies prior to committing `CMakeLists.txt` and initial headers.

Throughout Week 1, members utilized AI for C++ syntax clarification regarding CMake target linking and exception inheritance. Member 1 consulted ChatGPT to diagnose an `add_library` CMake configuration error caused by an empty `src/` directory, resulting in the creation of `src/version.cpp`. Member 2 utilized AI to clarify exception class inheritance syntax for `VizError` deriving from `std::runtime_error`. All other members worked directly from C++ documentation without AI assistance.

### 2.2 Week 2: Data Preprocessing, Drawing Primitives, and Math Debugging

Week 2 (12 October – 18 October 2026) focused on data cleaning pipelines, 2D rasterization algorithms, bitmapped fonts, and color spaces.

As team members implemented complex algorithms—such as Bresenham line drawing, 2D circle bounding checks, and ITU-R BT.601 perceived luminance calculations—AI tools served as interactive math tutors and compiler error decoders.

*   **Tool Utilized**: ChatGPT (OpenAI) and Claude (Anthropic).
*   **Purpose**: Explaining integer arithmetic overflow in Bresenham line error accumulation, clarifying bitwise shift operations (`(val >> 16) & 0xFF`) for 24-bit hex color parsing, and troubleshooting template instantiation syntax for `std::function` predicates in `filter_rows`.
*   **Reason**: Resolving subtle math bugs in pure C++ pixel manipulations required understanding underlying low-level mechanics without relying on external graphics libraries.
*   **Verification**: Member 7 verified line and circle rasterization by writing `tests/test_draw.cpp` and inspecting pixel outputs on a test `Canvas`. Member 9 verified color blending and luminance equations by executing unit checks in `tests/test_colormap.cpp`.

### 2.3 Week 3: Layout Integration, Figure Memory Management, and Plot Algorithms

Week 3 (19 October – 25 October 2026) represented the core statistical plot integration phase, introducing `Axes`, `Figure`, `PointPlot`, `SwarmPlot`, `Heatmap`, and `Colorbar`.

The primary technical challenge involved managing canvas memory lifecycle when returning `Figure` objects by value from functions, as well as collision-avoidance algorithms for swarm plots.

*   **Tool Utilized**: Claude (Anthropic).
*   **Purpose**: Diagnosing dangling canvas pointers in `Axes` caused by `Canvas` vector reallocation during move operations, and understanding non-parametric bootstrap resampling logic for confidence interval estimation.
*   **Reason**: C++17 move semantics and pointer stability required precise architectural design to prevent memory corruption when transferring plot figures.
*   **Verification**: Member 1 refactored `Figure` to store `Canvas` on the heap via `std::unique_ptr<Canvas>`, ensuring address stability. Member 5 verified bootstrap confidence intervals using fixed random seeds (`std::mt19937`) against hand-calculated statistical samples in `tests/test_stats.cpp`.

### 2.4 Week 4: Quick API Design, Cross-Testing, and Hardening

Week 4 (26 October – 1 November 2026) focused on user convenience wrappers (`quick.hpp`), independent cross-testing, stress testing, and application examples.

During this week, members engaged in cross-testing assigned modules owned by teammates to fulfill Course Instruction 7 (Collaboration and Independent Code Review).

*   **Tool Utilized**: ChatGPT (OpenAI).
*   **Purpose**: Generating diagnostic edge-case test ideas (such as zero-variance numeric columns, 10,000-row CSV stress testing, and ragged correlation matrices) to stress-test peer modules.
*   **Reason**: Independent peer review required rigorous test cases to ensure that public library functions throw appropriate `DataError` or `InvalidArgument` exceptions rather than crashing.
*   **Verification**: Members manually authored cross-module test executables (such as `test_swarmplot_layout.cpp` and `test_invalid_inputs.cpp`) using `mini_test.hpp` and verified that all test targets passed green under `ctest`.

### 2.5 Week 5: Release Verification, Build Hardening, and Final Submission

Week 5 (2 November – 5 November 2026) enforced a strict feature freeze dedicated to fresh-machine build verification, hand-typed README documentation, and final release tagging (`v1.0.0`).

No AI tools were utilized to generate library code or README content during Week 5. The README was hand-typed by all ten group members in accordance with Course Instruction 5. AI usage was limited to reviewing formatting consistency across progress reports.

---

## 3. Individual Member AI Usage & Code Ownership Matrix

To establish individual accountability and ensure defense readiness for the final oral examination, the table below documents the specific module ownership, AI tools consulted, purpose of consultation, and verification methods for all ten group members.

| Member | Owned Repository Files | AI Tool Used | Specific Purpose & Query Scope | Human Verification & Defense Readiness |
| :--- | :--- | :--- | :--- | :--- |
| **Member 1** (Kimuli Joseph) | `CMakeLists.txt`, `cppviz.hpp`, `figure.hpp/cpp`, `quick.hpp/cpp` | Claude (Anthropic) | Project folder skeleton, CMake target globbing, and `unique_ptr` move semantics in `Figure`. | Manually authored `Figure` heap allocation and verified CMake builds across Linux and Windows environments. |
| **Member 2** (Nangosha Anne Mary) | `types.hpp`, `error.hpp/cpp`, `validation.hpp/cpp`, `axes.hpp/cpp` | ChatGPT (OpenAI) | C++ exception class inheritance syntax (`VizError`) and inverted Y-axis tick mapping logic. | Hand-typed error hierarchy and verified pixel mapping boundaries in `test_axes.cpp`. |
| **Member 3** (Nakibuule Maria Liz) | `dataset.hpp/cpp`, `groupby.hpp/cpp`, `labels.hpp/cpp` | None | No AI tools utilized for code logic; standard C++ library documentation consulted. | Authored `Dataset` table storage and verified first-appearance category ordering in `test_groupby.cpp`. |
| **Member 4** (Anna Kagemulo Mtabazi) | `csv_loader.hpp/cpp`, `summary.hpp/cpp`, `annotation.hpp/cpp` | Claude (Anthropic) | State-machine logic for parsing quoted CSV cells containing embedded commas. | Hand-wrote CSV tokenizer and verified quoted string parsing across edge cases in `test_csv_loader.cpp`. |
| **Member 5** (Rwotolara Sunday Junior) | `stats.hpp/cpp`, `pointplot.hpp/cpp` | ChatGPT (OpenAI) | Understanding non-parametric bootstrap resampling algorithm and quantile interpolation. | Authored `bootstrap_ci` using `std::mt19937` and verified confidence interval bounds against known data. |
| **Member 6** (Kwagala Rebecca) | `scale.hpp/cpp`, `ticks.hpp/cpp`, `swarmplot.hpp/cpp` | Claude (Anthropic) | "Nice numbers" scale tick generation algorithm and collision-avoidance offset candidate search. | Hand-implemented Bresenham-like swarm point offset layout and validated non-overlapping point distances. |
| **Member 7** (Amviiko Rebecca Natasha) | `canvas.hpp/cpp`, `draw.hpp/cpp`, `heatmap.hpp/cpp` | ChatGPT (OpenAI) | Bresenham line error term formulation and 2D matrix cell bounding coordinate math. | Hand-typed pixel indexing (`(y*w + x)*3`) and verified circle and rectangle rendering on test canvases. |
| **Member 8** (Kalule Wegna) | `font.hpp/cpp`, `theme.hpp/cpp`, `image_export.hpp/cpp` | Claude (Anthropic) | BMP 54-byte binary header field alignment, little-endian word packing, and row padding formulas. | Authored 5x7 ASCII bitmap glyph map and verified binary BMP header structure using binary file streams. |
| **Member 9** (Kadama Humphrey Arthur) | `colormap.hpp/cpp`, `colorbar.hpp/cpp` | ChatGPT (OpenAI) | Bitwise shift mask formulas for hex color parsing and ITU-R BT.601 perceived luminance weighting. | Implemented multi-stop colormap blending, vertical colorbars, and verified text contrast selection. |
| **Member 10** (Kalema Titus Larry) | `mini_test.hpp`, `text_util.hpp/cpp`, `preprocess.hpp/cpp`, `legend.hpp/cpp` | None | No AI tools utilized for core logic; C++ preprocessor macro documentation consulted. | Authored `mini_test.hpp` macro framework (`CHECK_EQ`, `CHECK_THROWS`) used by the entire team. |

---

## 4. Technical Verification & Independent Code Defense Protocols

To ensure that AI assistance never substituted for student learning or individual mastery, we enforced three rigorous verification controls throughout the project lifecycle:

1.  **Independent Compilation & CTest Validation**: No code snippet or structural design suggested during AI consulting was committed to the repository without passing local compilation (`cmake --build build`) and unit test execution (`ctest`). The repository contains 30 dedicated unit test executables in `tests/`, ensuring 100% coverage of all public functions, edge cases, and invalid inputs.
2.  **Mandatory Peer Code Reviews**: Every pull request submitted to `main` required approval from Member 1. Reviewers were required to build the author's branch locally, verify that all tests passed, inspect the code for clarity and absence of copy-pasted AI artifacts, and confirm that public functions were properly documented.
3.  **Oral Examination Readiness**: Every group member maintains complete ownership over their designated files and is prepared to explain line-by-line implementation choices, algorithmic complexity, data structure selections, and memory management decisions during the final oral presentation. Furthermore, each member cross-reviewed an adjacent module, enabling them to defend integrated subsystem workflows.

---

## 5. Formal Declaration & Authorization Sign-off

We, the undersigned members of Group 6, hereby declare that this document represents a complete, true, and accurate record of all AI tool usage during the development of the `cppviz` C++ Data Visualization Library.

We confirm that:
*   All library source code in `src/`, headers in `include/cppviz/`, test executables in `tests/`, and user documentation in `README.md` were written and hand-typed by the team members.
*   AI tools were utilized strictly within the boundaries of course guidelines as educational and planning aids.
*   Every group member fully understands the design, implementation, and mathematical foundation of their contributed modules and is fully prepared to defend their work during examination.

| Member No. | Member Name | Role & Ownership | Declaration Confirmation |
| :--- | :--- | :--- | :--- |
| **Member 1 (Lead)** | KIMULI JOSEPH | Build Integration, Figure, Quick API | Confirmed & Verified |
| **Member 2** | NANGOSHA ANNE MARY | Shared Types, Exceptions, Axes Frame | Confirmed & Verified |
| **Member 3** | NAKIBUULE MARIA LIZ | Dataset Table, Grouping, Titles & Labels | Confirmed & Verified |
| **Member 4** | ANNA KAGEMULO MTABAZI | CSV Parser, Summaries, Annotations | Confirmed & Verified |
| **Member 5** | RWOTOLARA SUNDAY JUNIOR | Statistics, Bootstrap CIs, Point Plot | Confirmed & Verified |
| **Member 6** | KWAGALA REBECCA | Scale Hierarchy, Ticks, Swarm Plot | Confirmed & Verified |
| **Member 7** | AMVIIKO REBECCA NATASHA | Canvas Storage, 2D Shapes, Heatmap | Confirmed & Verified |
| **Member 8** | KALULE WEGNA | Bitmap Font, Themes, Image Export | Confirmed & Verified |
| **Member 9** | KADAMA HUMPHREY ARTHUR | Colormaps, Palettes, Colorbar | Confirmed & Verified |
| **Member 10** | KALEMA TITUS LARRY | Test Framework, String Utils, Preprocessing, Legend | Confirmed & Verified |

*Date of Declaration Sign-off: 4 November 2026*