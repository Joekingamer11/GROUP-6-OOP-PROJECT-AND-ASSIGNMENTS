# Week 1 Progress Report

## Completed
- Received the C++ group project instructions and the Group 6 allocation: a Seaborn-style data visualization library in C++ (data loading, preprocessing, statistical summaries, swarm plots, point plots, heatmaps, axis scaling, labels and annotations, plot rendering, image export).
- Read the instructions as a group and listed the requirements: reusable library, folder structure, tests, hand-typed README, Git/GitHub workflow, weekly reports, AI declaration, deadline 5 November 2026.
- Agreed a five-week plan with ten members: a weekly schedule, the files each member owns each week, and the function signatures the modules will share. The plan was drafted with AI help (see AI Use) and reviewed by the team.
- Named the library **INTRIVIZ** and added the `INTRIVIZ LIBRARY` folder to the group repository on 3 October (commit "FIRST COMMIT"). It contains the standard project skeleton:
  - `CMakeLists.txt` (C++17; builds the library, examples and tests; install rules; target and macros named after INTRIVIZ)
  - `LICENSE` (MIT; copyright line edited by the team) and `.gitignore`
  - `README.md` with the group members table and an outline of the sections to be written
  - `.gitkeep` placeholders that keep the empty folders `data/input`, `data/output`, `examples`, `include/intriviz`, `reports/team`, `src` and `tests`
- Fixed the order of the group members table in the README, with Member 1 as team lead.


## Challenges/Blockers
- "Before the beginning of the week" can be read two ways for the weekly reports. We are submitting each report on the Sunday before the week it covers.
- Most of us are beginners with C++ and Git, so the first week includes practice with branches and pull requests.
- Managing 10 different contributors through pull requests will make it hard to avoid merge conflicts.

## Next Week
- Week 2 coding (5 to 11 October), one segment per member (numbers follow the members table in the README):
  - Member 1: `CMakeLists.txt` checks, `src/version.cpp`, umbrella header
  - Member 2: `types.hpp`, `error.hpp`
  - Member Nakibuule Maria Liz: `Dataset` class
  - Member 4: CSV loader
  - Member 5: basic statistics
  - Member 6: `Scale` and `LinearScale`
  - Member 7: `Canvas`
  - Member 8: BMP and PPM image export
  - Member 9: colour parsing, palette and `Colormap`
  - Member 10: test helper (`mini_test.hpp`) and text utilities
- Each member opens a pull request by Thursday; reviews on Friday; Member 1 merges on Saturday.
- Goal: a fresh clone builds with CMake, `ctest` runs, and a canvas can be saved as a BMP image.

## AI Use
- Tool: Claude (Anthropic)
- Reason: We are beginners and needed a clear plan and a shared structure so ten people could work in parallel. The team reviewed and adjusted the output, and the member who owns each file will be able to explain it.
