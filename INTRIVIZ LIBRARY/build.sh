#!/usr/bin/env bash
# Build INTRIVIZ (library + examples + tests) and run the tests.
# Usage (from the "INTRIVIZ LIBRARY" folder, in Git Bash):  ./build.sh
set -e
cd "$(dirname "$0")"

GEN=()
case "$(uname -s)" in
  MINGW*|MSYS*|CYGWIN*) GEN=(-G "MinGW Makefiles") ;;   # Git Bash on Windows
esac

rm -rf build
cmake -S . -B build "${GEN[@]}" -DINTRIVIZ_BUILD_EXAMPLES=ON -DINTRIVIZ_BUILD_TESTS=ON
cmake --build build -j4
ctest --test-dir build --output-on-failure
