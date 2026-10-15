#include "mini_test.hpp"
#include <stdexcept>

int main() {
    // Basic assertion macro checks
    CHECK(1 + 1 == 2);
    CHECK_EQ(10, 10);
    CHECK_NEAR(3.14159, 3.14150, 0.001);

    // Exception matching test
    CHECK_THROWS(throw std::runtime_error("sample error"), std::runtime_error);

    return finish();
}