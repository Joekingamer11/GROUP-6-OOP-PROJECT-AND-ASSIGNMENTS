#include "mini_test.hpp"
#include "cppviz/error.hpp"

int main() {
    using namespace cppviz;

    // Test require() helper behavior
    CHECK_THROWS(require(false, "invalid input"), InvalidArgument);
    require(true, "valid condition");

    // Test inheritance hierarchy
    bool caught_data_error = false;
    try {
        throw DataError("column not found");
    } catch (const VizError& e) {
        caught_data_error = true;
    }
    CHECK(caught_data_error);

    bool caught_invalid_arg = false;
    try {
        throw InvalidArgument("invalid option");
    } catch (const VizError& e) {
        caught_invalid_arg = true;
    }
    CHECK(caught_invalid_arg);

    bool caught_io_error = false;
    try {
        throw IoError("file open failed");
    } catch (const VizError& e) {
        caught_io_error = true;
    }
    CHECK(caught_io_error);

    return finish();
}