#include <catch2/catch.hpp>
#include <samwarring/ring_buffer.hpp>
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

        SECTION("iteration") {
            std::vector<int> expected{0, 0, 1, 2};
            REQUIRE(std::equal(buf.begin(), buf.end(), expected.begin()));
        }

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
                REQUIRE(buf.capacity() == 4);
                REQUIRE(buf[0] == 2);
                REQUIRE(buf[1] == 3);
                REQUIRE(buf[2] == 4);
                REQUIRE(buf[3] == 5);
            }

            SECTION("copy-constructed buffer values") {
                REQUIRE(buf2.capacity() == 4);
                REQUIRE(buf2[0] == 3);
                REQUIRE(buf2[1] == 4);
                REQUIRE(buf2[2] == 5);
                REQUIRE(buf2[3] == 7);
            }
        }

        SECTION("move constructor") {
            ring_buffer<int> buf2{std::move(buf)};
            REQUIRE(buf.capacity() == 0);
            REQUIRE(buf2.capacity() == 4);
            REQUIRE(buf2[0] == 2);
            REQUIRE(buf2[1] == 3);
            REQUIRE(buf2[2] == 4);
            REQUIRE(buf2[3] == 5);
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
