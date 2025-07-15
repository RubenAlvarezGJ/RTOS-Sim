#include <catch2/catch_test_macros.hpp>

// just making sure the testing framework is working; this is just a sample test.

int add(int a, int b) {
    return a + b;
}

TEST_CASE("Simple addition", "[add]") {
    REQUIRE(add(2, 3) == 5);
    REQUIRE(add(-1, 1) == 0);
}