#pragma once

#include <stdexcept>
#include <string>

namespace intriviz {

// Base exception class for the intriviz library
class VizError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Throw for invalid parameters or arguments (e.g. size mismatch, invalid option, out-of-range bounds).
class InvalidArgument : public VizError {
public:
    using VizError::VizError;
}; // bad function input

// Throw for dataset problems (e.g. missing column, wrong columntype, empty dataset).
class DataError : public VizError {
public:
    using VizError::VizError;
}; // missing column, wrong type, empty data

// Throw for file opening or writing failures.
class IoError : public VizError {
public:
    using VizError::VizError;
}; // file cannot be opened or written

// Throws InvalidArgument(message) when condition is false.
void require(bool condition, const std::string& message);

} // namespace intriviz