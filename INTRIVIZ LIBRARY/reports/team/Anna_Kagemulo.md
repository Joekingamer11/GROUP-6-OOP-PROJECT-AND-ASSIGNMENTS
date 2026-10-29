# Weekly Progress Report

## Anna Kagemulo mtabazi (member 4)

### Week 1 Summary

This week, I focused on understanding the project requirements and contributing to the group’s early setup for the INTRIVIZ C++ library and did the csv_loader
files . 

### Tasks I Completed

- Worked on my assigned group task, the CSV loader module for the INTRIVIZ library.
- Helped implement the `load_csv` and `parse_csv` functionality for reading dataset files into the project’s `Dataset` structure.
- Learned and applied logic for handling:
  - CSV delimiter parsing
  - header detection
  - quoted values and commas inside fields
  - missing values and empty rows
  - validation of row length consistency
  - conversion of numeric and text columns into structured dataset values
- Reviewed the loader implementation and checked that it matched the project’s requirements and expected error handling.
- Documented my progress and kept track of the work completed this week.

### Challenges Faced

- As a beginner in C++, some of the project concepts required extra reading and practice.
- Understanding the full team workflow and how different members’ tasks would connect was initially challenging.


### AI use
- Tool: microsoft co pilot (Anthropic)
- Reason: We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.

### Week 2 Summary Report

### Tasks I Completed
- Worked on my assigned group task, the summary module ie summary.cpp , summary.hpp and tests_summary.cpp .
- Reviewed the loader implementation and checked that it matched the project’s requirements and expected error handling.
- Documented my progress and kept track of the work completed this week.

### AI use
- Tool: microsoft co pilot (Anthropic) and google gemini
- Reason: We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.

 ### Week 2 Summary Report

### Tasks I Completed
- Worked on my assigned group task, the summary module ie summary.cpp , summary.hpp and tests_summary.cpp .
- Reviewed the loader implementation and checked that it matched the project’s requirements and expected error handling.
- Documented my progress and kept track of the work completed this week.

### AI use
- Tool: microsoft co pilot (Anthropic) and google gemini
- Reason: We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.



### Week 3 Summary Report

### Tasks I Completed
- Worked on the annotation module in `annotation.cpp` and `annotation.hpp`, and its tests in `test_annotation.cpp`.
- Implemented text annotations positioned at data coordinates.
- Implemented horizontal and vertical reference lines, clipped to the plot area and customizable by color and thickness.
- Implemented arrows between data coordinates, including arrowheads and optional text at the arrow tail.
- Checked that annotation functions require both axes scales to be set and throw `InvalidArgument` otherwise.
- Tested reference-line rendering, clipping of out-of-bounds lines, and arrow rendering.
- Documented my progress and kept track of the work completed this week.

### AI use
- Tool: Microsoft Copilot (Anthropic) and Google Gemini
- Reason: We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.

# Week 4 Summary Report 

### Tasks Completed
- Created the sample dataset fixture (`data/input/scores_sample.csv`) containing 30 rows of categorical student groups and test scores.
- Implemented the high-level point plot executable example (`examples/example_pointplot.cpp`) demonstrating `quick_pointplot` and file export.
- Implemented the low-level custom annotations executable example (`examples/example_annotations.cpp`) demonstrating manual `Figure` construction, reference benchmark lines (`annotate_hline`), and arrow notes (`annotate_arrow`).
- Added comprehensive Doxygen documentation comments across all owned public headers (`csv_loader.hpp`, `summary.hpp`, and `annotation.hpp`), detailing parameters, return values, default options, and exception conditions (`DataError`, `IoError`, `InvalidArgument`).
- Hardened `src/annotation.cpp` by resolving edge-case boundary clipping issues discovered during example image testing.

### AI Use
- **Tool:** Microsoft Copilot (Anthropic) and Google Gemini
- **Purpose:** Assisting with example code layout, Doxygen documentation formatting, and report drafting.
- **Reason:** We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.