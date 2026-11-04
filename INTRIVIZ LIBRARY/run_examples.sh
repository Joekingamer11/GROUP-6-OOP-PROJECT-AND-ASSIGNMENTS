#!/usr/bin/env bash
# Run every example program. Usage (after ./build.sh):  ./run_examples.sh
cd "$(dirname "$0")"          # examples write to data/output, so run from the project root
mkdir -p data/output

pass=0; fail=0
for src in examples/*.cpp; do
  name=$(basename "$src" .cpp)
  exe=""
  for p in "build/$name" "build/$name.exe" "build/Release/$name.exe"; do
    [ -f "$p" ] && exe="$p" && break
  done
  if [ -z "$exe" ]; then echo "SKIP  $name (not built)"; fail=$((fail+1)); continue; fi
  echo "=== $name ==="
  if "./$exe"; then echo "OK    $name"; pass=$((pass+1)); else echo "FAIL  $name"; fail=$((fail+1)); fi
done
echo; echo "Passed: $pass   Failed/skipped: $fail"
echo "Images are in data/output/"
[ "$fail" -eq 0 ]
