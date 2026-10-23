#include "mini_test.hpp"
#include "cppviz/text_util.hpp"

int main() {
    using namespace cppviz;

    // Test trim
    CHECK_EQ(trim("  a b \t\r\n"), "a b");
    CHECK_EQ(trim(""), "");
    CHECK_EQ(trim("   "), "");

    // Test split
    auto parts = split("a,b,,c", ',');
    CHECK_EQ(parts.size(), 4u);
    CHECK_EQ(parts[0], std::string("a"));
    CHECK_EQ(parts[1], std::string("b"));
    CHECK_EQ(parts[2], std::string(""));
    CHECK_EQ(parts[3], std::string("c"));

    // Test to_lower
    CHECK_EQ(to_lower("Hello WORLD 123!"), "hello world 123!");

    // Test try_parse_double
    double val = 0.0;
    CHECK(try_parse_double("3.5", val));
    CHECK_NEAR(val, 3.5, 1e-9);

    CHECK(try_parse_double("1e3", val));
    CHECK_NEAR(val, 1000.0, 1e-9);

    // Invalid number strings must return false
    CHECK(!try_parse_double("3.5abc", val));
    CHECK(!try_parse_double("", val));
    CHECK(!try_parse_double("abc", val));

    return finish();
}