#include <catch2/catch.hpp>
#include <samwarring/ring_buffer.hpp>
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
        REQUIRE(buf.front() == 1);
        REQUIRE(buf.back() == 4);
        REQUIRE(buf[0] == 1);
        REQUIRE(buf[1] == 2);
        REQUIRE(buf[2] == 3);
        REQUIRE(buf[3] == 4);
    }

    SECTION("iteration") {
        std::vector<int> expected{1, 2, 3, 4};
        REQUIRE(std::equal(buf.cbegin(), buf.cend(), expected.begin()));
    }
}
