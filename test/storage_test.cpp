#include <catch2/catch.hpp>
#include <samwarring/instance_tracker.hpp>
#include <samwarring/storage.hpp>

using namespace samwarring;

TEST_CASE("storage") {
    storage<int> i;
    i.construct();

    storage<double> j;
    j.construct(3.14);

    // storage<instance_tracker> k;
    // k.construct();
}