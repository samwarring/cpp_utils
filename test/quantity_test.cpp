#include <catch2/catch_all.hpp>
#include <samwarring/quantity.hpp>

using namespace samwarring::units;

TEST_CASE("quantity") {
    seconds<double> _120_sec{120.0};
    minutes<double> _2_min{2.0};
    grams<double> _123_grams{123.0};
    kilograms<double> _0_123_kg{0.123};
    degrees<double> _90_deg{90.0};
    radians<double> _pi_2_rad{std::numbers::pi / 2.0};

    
    meters<double> _55_meters{55.0};
    millimeters<double> _55000_millimeters{55000.0};
    auto sq_meters = _55_meters * _55_meters;
    auto sq_millimeters = _55000_millimeters * _55000_millimeters;
    REQUIRE(sq_meters == sq_millimeters);

    auto q1 = sq_meters / _55000_millimeters;
    auto q2 = sq_millimeters / _55_meters;
    REQUIRE(q1 == _55_meters);
    REQUIRE(q1 == _55000_millimeters);
    REQUIRE(q2 == _55_meters);
    REQUIRE(q2 == _55000_millimeters);

    REQUIRE(_120_sec == _2_min);
    REQUIRE(_123_grams == _0_123_kg);
    REQUIRE(_90_deg == _pi_2_rad);
}