#pragma once

#include <stdexcept>
#include <string>

namespace cppviz {

// Base exception class for the cppviz library
class VizError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidArgument : public VizError {
public:
    using VizError::VizError;
}; // bad function input

class DataError : public VizError {
public:
    using VizError::VizError;
}; // missing column, wrong type, empty data

class IoError : public VizError {
public:
    using VizError::VizError;
}; // file cannot be opened or written

// Throws InvalidArgument(message) when condition is false.
void require(bool condition, const std::string& message);

} // namespace cppviz