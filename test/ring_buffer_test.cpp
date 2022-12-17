#include <catch2/catch.hpp>
#include <samwarring/instance_tracker.hpp>
#include <samwarring/ring_buffer.hpp>
#include <samwarring/singleton.hpp>
#include <set>
#include <string>
#include <vector>

using namespace samwarring;

TEST_CASE("ring buffer") {
    ring_buffer<int> buf{4};

    SECTION("without rollover") {
        buf.push_back(1);
        buf.push_back(2);
        REQUIRE(buf.front() == 0);
        REQUIRE(buf.back() == 2);
        REQUIRE(buf[0] == 0);
        REQUIRE(buf[1] == 0);
        REQUIRE(buf[2] == 1);
        REQUIRE(buf[3] == 2);

        SECTION("modification") {
            buf.front() = 10;
            buf[1] = 20;
            buf.back() = 30;
            REQUIRE(buf[0] == 10);
            REQUIRE(buf[1] == 20);
            REQUIRE(buf[2] == 1);
            REQUIRE(buf[3] == 30);
        }
    }

    SECTION("with rollover") {
        buf.push_back(1);
        buf.push_back(2);
        buf.push_back(3);
        buf.push_back(4);
        buf.push_back(5);
        REQUIRE(buf.front() == 2);
        REQUIRE(buf.back() == 5);
        REQUIRE(buf[0] == 2);
        REQUIRE(buf[1] == 3);
        REQUIRE(buf[2] == 4);
        REQUIRE(buf[3] == 5);

        SECTION("iteration") {
            std::vector<int> expected{2, 3, 4, 5};
            REQUIRE(std::equal(buf.begin(), buf.end(), expected.begin()));
        }

        SECTION("bidirectional iteration") {
            auto it = buf.begin();
            REQUIRE(*it == 2);
            REQUIRE(*(--it) == 5);
            REQUIRE(*(++it) == 2);
        }

        SECTION("unordered iteration") {
            std::set<int> expected{2, 3, 4, 5};
            std::set<int> actual{buf.unordered_begin(), buf.unordered_end()};
            REQUIRE(std::equal(actual.begin(), actual.end(), expected.begin()));

            SECTION("const unordered iteration") {
                const ring_buffer<int>& cref = buf;
                std::set<int> actual_unordered{cref.unordered_begin(),
                                               cref.unordered_end()};
                REQUIRE(std::equal(actual_unordered.begin(),
                                   actual_unordered.end(), expected.begin()));
            }
        }

        SECTION("for_each iteration") {
            std::vector<int> expected{2, 3, 4, 5};
            std::vector<int> actual;
            buf.for_each([&](int val) { actual.push_back(val); });
            REQUIRE(std::equal(actual.begin(), actual.end(), expected.begin()));
        }

        SECTION("copy constructor") {
            ring_buffer<int> buf2{buf};
            buf2.push_back(7);

            SECTION("original buffer unmodified") {
                REQUIRE(buf.size() == 4);
                REQUIRE(buf[0] == 2);
                REQUIRE(buf[1] == 3);
                REQUIRE(buf[2] == 4);
                REQUIRE(buf[3] == 5);
            }

            SECTION("copy-constructed buffer values") {
                REQUIRE(buf2.size() == 4);
                REQUIRE(buf2[0] == 3);
                REQUIRE(buf2[1] == 4);
                REQUIRE(buf2[2] == 5);
                REQUIRE(buf2[3] == 7);
            }
        }

        SECTION("move constructor") {
            ring_buffer<int> buf2{std::move(buf)};
            REQUIRE(buf.size() == 0);
            REQUIRE(buf2.size() == 4);
            REQUIRE(buf2[0] == 2);
            REQUIRE(buf2[1] == 3);
            REQUIRE(buf2[2] == 4);
            REQUIRE(buf2[3] == 5);
        }

        SECTION("copy assignment") {
            ring_buffer<int> buf2{6};
            buf = buf2;
            REQUIRE(buf.size() == 6);
            REQUIRE(buf2.size() == 6);
            buf[0] = 10;
            buf2[0] = 20;
            REQUIRE(buf[0] == 10);
            REQUIRE(buf2[0] == 20);
        }

        SECTION("move assignment") {
            ring_buffer<int> buf2{7};
            buf2[0] = 123;
            buf = std::move(buf2);
            REQUIRE(buf.size() == 7);
            REQUIRE(buf[0] == 123);
        }
    }
}

TEST_CASE("ring buffer of strings") {
    ring_buffer<std::string> buf{4};

    buf.push_back("poodle monkey");
    buf.push_back("platypus bear");
    buf.push_back("tigerdillo");
    buf.push_back("flying bison");
    buf.push_back("elephant koi");

    SECTION("iteration") {
        std::vector<std::string> expected{"platypus bear", "tigerdillo",
                                          "flying bison", "elephant koi"};
        REQUIRE(std::equal(buf.begin(), buf.end(), expected.begin()));
    }
}

TEST_CASE("const ring buffer") {
    ring_buffer<int> buf{4};
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);
    buf.push_back(4);
    const ring_buffer<int>& cbuf = buf;

    SECTION("access") {
        REQUIRE(cbuf.front() == 1);
        REQUIRE(cbuf.back() == 4);
        REQUIRE(cbuf[0] == 1);
        REQUIRE(cbuf[1] == 2);
        REQUIRE(cbuf[2] == 3);
        REQUIRE(cbuf[3] == 4);
    }

    SECTION("iteration") {
        std::vector<int> expected{1, 2, 3, 4};
        REQUIRE(std::equal(cbuf.begin(), cbuf.end(), expected.begin()));
    }
}

TEST_CASE("ring buffer iterator") {
    ring_buffer<int> buf{4};
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);
    buf.push_back(4);
    buf.push_back(5);
    std::vector<int> expected{2, 3, 4, 5};
    std::vector<int> expected_rev{5, 4, 3, 2};

    SECTION("forward traversal") {
        REQUIRE(std::equal(buf.begin(), buf.end(), expected.begin(),
                           expected.end()));
    }

    SECTION("backward traversal") {
        std::vector<int> actual;
        auto it = buf.end();
        while (it != buf.begin()) {
            --it;
            actual.push_back(*it);
        }
        REQUIRE(std::equal(actual.begin(), actual.end(), expected_rev.begin(),
                           expected_rev.end()));
    }

    SECTION("std::reverse") {
        std::reverse(buf.begin(), buf.end());
        REQUIRE(std::equal(buf.begin(), buf.end(), expected_rev.begin(),
                           expected_rev.end()));
    }

    SECTION("adjust by offset") {
        REQUIRE(buf.begin() + buf.size() == buf.end());
        REQUIRE(buf.end() - buf.size() == buf.begin());
        auto it = buf.begin(); // [(2), 3, 4, 5]
        it += 2;               // [2, 3, (4), 5]
        REQUIRE(*it == 4);
        it += 1; // [2, 3, 4, (5)]
        REQUIRE(*it == 5);
        it -= 2; // [2, (3), 4, 5]
        REQUIRE(*it == 3);
        REQUIRE(*(it + 0) == 3);
        REQUIRE(*(it - 0) == 3);
        it += 0;
        REQUIRE(*it == 3);
        it -= 0;
        REQUIRE(*it == 3);
    }

    SECTION("iterator difference") {
        REQUIRE(buf.begin() - buf.begin() == 0);
        REQUIRE(buf.end() - buf.end() == 0);
        auto it1 = buf.begin() + 1;
        auto it3 = buf.begin() + 3;
        REQUIRE(it1 - it3 == -2);
        REQUIRE(it3 - it1 == 2);
    }

    SECTION("iterator comparison") {
        REQUIRE(buf.begin() < buf.end());
        REQUIRE(buf.begin() <= buf.end());
        REQUIRE(buf.begin() <= buf.begin());
        REQUIRE(buf.end() - 1 < buf.end());
        REQUIRE(buf.end() > buf.begin());
        REQUIRE(buf.end() >= buf.begin());
        REQUIRE(buf.end() >= buf.end());
    }

    SECTION("iterator indexing") {
        auto it = buf.begin();
        REQUIRE(it[0] == 2);
        REQUIRE(it[1] == 3);
        REQUIRE(it[2] == 4);
        REQUIRE(it[3] == 5);

        auto it2 = buf.end();
        REQUIRE(it[-1] == 5);
        REQUIRE(it[-2] == 4);
        REQUIRE(it[-3] == 3);
        REQUIRE(it[-4] == 2);
    }
}

TEST_CASE("ring buffer instance tracker") {
    auto stats = reference_counted_singleton<instance_tracker_stats>();
    ring_buffer<instance_tracker> buf{10};
    std::set<int> ids;
    buf.for_each([&](auto& tracker) { ids.insert(tracker.id()); });

    SECTION("items initialized with default constructor") {
        REQUIRE(stats->default_constructors == 10);
        REQUIRE(stats->all_constructors == 10);
        REQUIRE(stats->instances == 10);
    }

    SECTION("copy assignment") {
        ring_buffer<instance_tracker> buf2{20};
        REQUIRE(stats->instances == 30);
        REQUIRE(stats->destructors == 0);
        buf = buf2;

        // Now, buf with 20, and buf2 with 20. The original 10 instances in buf
        // were destroyed.
        REQUIRE(stats->instances == 40);
        REQUIRE(stats->destructors == 10);
        REQUIRE(stats->destroyed_ids == ids);
    }

    SECTION("move assignment") {
        ring_buffer<instance_tracker> buf2{100};
        REQUIRE(stats->instances == 110);
        REQUIRE(stats->destructors == 0);
        buf = std::move(buf2);

        // Now, buf with 100, buf2 with 0. The original 10 instances in buf were
        // destroyed.
        REQUIRE(stats->instances == 100);
        REQUIRE(stats->destructors == 10);
        REQUIRE(stats->destroyed_ids == ids);
    }
}
