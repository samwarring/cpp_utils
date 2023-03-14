#include <catch2/catch.hpp>
#include <samwarring/quantity.hpp>

using namespace samwarring::units;

TEST_CASE("quantity") {
    seconds<double> s{23.0};
    grams<double> g{42.0};
    // seconds2 oops{g}; // will not compile
    meters<double> m{55.55};
    kilograms<double> kg{g};
    REQUIRE(kg.value() == 0.042);

    REQUIRE(seconds<int>{minutes<int>{2}}.value() == 120);
    REQUIRE((seconds<int>{4} * 2).value() == 8);
    REQUIRE((meters<int>{3} * meters<int>{2}).value() == 6);
    REQUIRE(seconds<int>{120} == minutes<int>{2});

    bool b = radians<double>{std::numbers::pi / 2.0} == degrees<double>{90.0};
    REQUIRE(b);
}