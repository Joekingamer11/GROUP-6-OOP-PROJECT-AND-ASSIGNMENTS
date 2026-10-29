#include "intriviz/error.hpp"
#include "intriviz/types.hpp"

namespace intriviz {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw InvalidArgument(message);
    }
}

bool operator==(const Color& a, const Color& b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

bool operator!=(const Color& a, const Color& b) {
    return !(a == b);
}

} // namespace intriviz