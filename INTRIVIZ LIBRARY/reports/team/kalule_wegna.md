# Week 1

## Files added

### `include/cppviz/image_export.hpp`

This file declares three functions that save a canvas as an image: `write_ppm`, `write_bmp` and `save_image`. The first two write one specific format each. `save_image` picks the format from the file extension and throws `InvalidArgument` if it is anything other than `.ppm` or `.bmp`. All three take a `const Canvas&` so they do not change the picture they are given.

### `src/image_export.cpp`

This file implements the three functions.

`write_ppm` writes a P6 PPM file. It puts a small text header first (the magic number `P6`, the width and height, then `255`), then the raw RGB bytes of every pixel going row by row. PPM is the simplest image format, so it is the easiest one to write and test.

`write_bmp` writes a 24-bit BMP file. It writes a 14-byte file header and a 40-byte info header, then the pixels. Every number in the header is written little-endian, one byte at a time, using two small helpers called `write_u16` and `write_u32`. Those helpers live inside an anonymous namespace so they are not part of the public API. The pixels follow three rules the BMP format needs: rows go bottom to top, each pixel is written in blue-green-red order instead of red-green-blue, and each row is padded with zero bytes until its length is a multiple of four.

`save_image` checks the last four characters of the path. If they are `.ppm` it calls `write_ppm`, if they are `.bmp` it calls `write_bmp`, otherwise it throws `InvalidArgument` with the path in the message. The two writers throw `IoError` if the file cannot be opened for writing.

### `tests/test_image_export.cpp`

This file tests the three functions with normal input, edge cases, and invalid input.

It makes a small 3x2 canvas with two coloured pixels, writes it with `write_ppm`, opens the file and checks it starts with the P6 magic number. Then it writes the same canvas with `write_bmp` and checks the first two bytes are `B` and `M`. Both files are deleted at the end so nothing is left behind. Finally it checks that `save_image` throws `InvalidArgument` when the extension is something like `.gif`.

The tests use the shared macros from `tests/mini_test.hpp`: `CHECK`, `CHECK_EQ` and `CHECK_THROWS`. They end with `return finish();` so `ctest` sees a failure if any check fails.

## How the files work together

The header is the public interface other parts of the library call. The source file has the implementation. The tests go through the public interface only, so they would still catch a problem if the internal helpers changed.

`image_export` is the last stage of the pipeline in the README. Every plot gives us a `Canvas`, and `image_export` turns that canvas into a file the user can open.

## AI use

- Tool: Notebook AI
- Purpose: Helped with the PPM and BMP writers, mainly the BMP header bytes and the bottom-up, BGR, row-padded pixel layout.
- Reason: The BMP header fields and pixel order are easy to get wrong from memory. Having a worked example made it easier to follow.

The three files were created by me. AI was used as a supporting tool to help with the code, mostly for the BMP byte layout. I did not copy a finished module. I read the code line by line, watched videos about PPM, BMP and how C++ writes binary files, and I am still learning the details so I can explain and defend each function in the presentation.