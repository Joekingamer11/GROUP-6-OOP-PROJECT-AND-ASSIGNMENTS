# Week 1

## Files added

### `include/cppviz/cppviz.hpp`

The public `cppviz` header declares the library's `cppviz::version()` function. It also includes the shared types and error headers, providing a central header for clients that need these common library interfaces.

### `src/version.cpp`

This source file implements `cppviz::version()`. It returns the library version as the string `"1.0.0"`, providing a single version value that can be queried by library users.

## How the files work together

The header exposes the version function to consumers of the library, while `version.cpp` supplies its implementation. The implementation is compiled into the library from `src/` by the project's CMake configuration.