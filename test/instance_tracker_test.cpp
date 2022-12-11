#include <catch2/catch.hpp>
#include <memory>
#include <samwarring/instance_tracker.hpp>

using namespace samwarring;

TEST_CASE("instance tracker") {
    auto stats = std::make_shared<instance_tracker_stats>();

    SECTION("tracks active instances") {
        REQUIRE(stats->instances == 0);
        instance_tracker t1(stats);
        REQUIRE(stats->instances == 1);
        {
            instance_tracker t2(stats);
            REQUIRE(stats->instances == 2);
        }
        REQUIRE(stats->instances == 1);
    }

    SECTION("copy constructor") {
        instance_tracker t1(stats);
        auto t1_id = t1.id();
        instance_tracker t2(t1);

        SECTION("stats updated") {
            REQUIRE(stats->copy_constructors == 1);
            REQUIRE(stats->all_constructors == 2);
            REQUIRE(stats->all_copies == 1);
        }

        SECTION("new instance gets unique ID") {
            REQUIRE(t2.id() != t1_id);
        }

        SECTION("copied-from instance retains ID") {
            REQUIRE(t1.id() == t1_id);
        }
    }

    SECTION("copy assignment") {
        instance_tracker t1(stats);
        instance_tracker t2(stats);
        auto t1_id = t1.id();
        auto t2_id = t2.id();
        t1 = t2;

        SECTION("stats updated") {
            REQUIRE(stats->copy_assignments == 1);
            REQUIRE(stats->all_assignments == 1);
            REQUIRE(stats->all_copies == 1);
        }

        SECTION("instances retain IDs") {
            REQUIRE(t1.id() == t1_id);
            REQUIRE(t2.id() == t2_id);
        }
    }

    SECTION("move constructor") {
        instance_tracker t1(stats);
        auto id = t1.id();
        instance_tracker t2(std::move(t1));

        SECTION("stats updated") {
            REQUIRE(stats->move_constructors == 1);
            REQUIRE(stats->all_constructors == 2);
            REQUIRE(stats->all_moves == 1);
        }

        SECTION("ID is moved") {
            REQUIRE(t1.id() == 0);
            REQUIRE(t2.id() == id);
        }
    }

    SECTION("move assignment") {
        instance_tracker t1(stats);
        instance_tracker t2(stats);
        auto t1_id = t1.id();
        auto t2_id = t2.id();
        REQUIRE(t1_id != t2_id);
        t2 = std::move(t1);

        SECTION("stats updated") {
            REQUIRE(stats->move_assignments == 1);
            REQUIRE(stats->all_assignments == 1);
            REQUIRE(stats->all_moves == 1);
        }

        SECTION("ID is moved") {
            REQUIRE(t1.id() == 0);
            REQUIRE(t2.id() == t1_id);
        }

        SECTION("moved-into instance is evicted") {
            REQUIRE(stats->evicted_ids.count(t2_id) == 1);
        }
    }
}