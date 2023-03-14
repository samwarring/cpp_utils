#include <catch2/catch_all.hpp>
#include <samwarring/quantity.hpp>

using namespace samwarring::units;

TEST_CASE("quantity") {
    seconds<double> _120_sec{120.0};
    minutes<double> _2_min{2.0};
    meters<double> _55_meters{55.0};
    grams<double> _123_grams{123.0};
    kilograms<double> _0_123_kg{0.123};
    degrees<double> _90_deg{90.0};
    radians<double> _pi_2_rad{std::numbers::pi / 2.0};

    // Unused
    (void)_55_meters;

    REQUIRE(_120_sec == _2_min);
    REQUIRE(_123_grams == _0_123_kg);
    REQUIRE(_90_deg == _pi_2_rad);
}