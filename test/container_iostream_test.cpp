#include <catch2/catch.hpp>
#include <samwarring/container_iostream.hpp>
#include <sstream>

using namespace samwarring::container_iostream;

SCENARIO("Containers can be formatted with ostream") {
    std::ostringstream out;

    // std::vector
    GIVEN("An empty vector") {
        std::vector<int> v;
        WHEN("The vector is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A vector with one element: 20") {
        std::vector<int> v;
        v.push_back(20);
        WHEN("The vector is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A vector with many elements: 1, 2, 3") {
        std::vector<int> v{1, 2, 3};
        WHEN("The vector is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::list
    GIVEN("An empty list") {
        std::list<int> v;
        WHEN("The list is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A list with one element: 20") {
        std::list<int> v;
        v.push_back(20);
        WHEN("The list is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A list with many elements: 1, 2, 3") {
        std::list<int> v{1, 2, 3};
        WHEN("The list is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::deque
    GIVEN("An empty deque") {
        std::deque<int> v;
        WHEN("The deque is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A deque with one element: 20") {
        std::deque<int> v;
        v.push_back(20);
        WHEN("The deque is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A deque with many elements: 1, 2, 3") {
        std::deque<int> v{1, 2, 3};
        WHEN("The deque is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::set
    GIVEN("An empty set") {
        std::set<int> v;
        WHEN("The set is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A set with one element: 20") {
        std::set<int> v;
        v.insert(20);
        WHEN("The set is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A set with many elements: 1, 2, 3") {
        std::set<int> v{1, 2, 3};
        WHEN("The set is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::unordered_set
    GIVEN("An empty unordered_set") {
        std::unordered_set<int> v;
        WHEN("The unordered_set is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A unordered_set with one element: 20") {
        std::unordered_set<int> v;
        v.insert(20);
        WHEN("The unordered_set is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A unordered_set with many elements: 1, 2, 3") {
        std::unordered_set<int> v{1, 2, 3};
        WHEN("The unordered_set is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::array
    GIVEN("An empty array") {
        std::array<int, 0> v;
        WHEN("The array is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A array with one element: 20") {
        std::array<int, 1> v{20};
        WHEN("The array is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("A array with many elements: 1, 2, 3") {
        std::array<int, 3> v{1, 2, 3};
        WHEN("The array is formatted") {
            out << v;
            THEN("The result is '{1, 2, 3}'") {
                REQUIRE(out.str() == "{1, 2, 3}");
            }
        }
    }

    // std::map
    GIVEN("An empty map") {
        std::map<int, int> v;
        WHEN("The map is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A map with one element: (20: 30)") {
        std::map<int, int> v{{20, 30}};
        WHEN("The map is formatted") {
            out << v;
            THEN("The result is '{20: 30}'") {
                REQUIRE(out.str() == "{20: 30}");
            }
        }
    }
    GIVEN("A map with many elements: (1: a, 2: b, 3: c)") {
        std::map<int, char> v{{1, 'a'}, {2, 'b'}, {3, 'c'}};
        WHEN("The map is formatted") {
            out << v;
            THEN("The result is '{1: a, 2: b, 3: c}'") {
                REQUIRE(out.str() == "{1: a, 2: b, 3: c}");
            }
        }
    }

    // std::pair
    GIVEN("A pair of ints: (1, 2)") {
        std::pair<int, int> v{1, 2};
        WHEN("The pair is formatted") {
            out << v;
            THEN("The result is '{1, 2}'") {
                REQUIRE(out.str() == "{1, 2}");
            }
        }
    }
    GIVEN("A pair of int-string: (42, 'the answer')") {
        std::pair<int, std::string> v{42, "the answer"};
        WHEN("The pair is formatted") {
            out << v;
            THEN("The result is '{42, the answer}'") {
                REQUIRE(out.str() == "{42, the answer}");
            }
        }
    }

    // std::tuple
    GIVEN("An empty tuple") {
        std::tuple<> v;
        WHEN("The tuple is formatted") {
            out << v;
            THEN("The result is '{}'") {
                REQUIRE(out.str() == "{}");
            }
        }
    }
    GIVEN("A 1-tuple: ('hello')") {
        std::tuple<std::string> v{"hello"};
        WHEN("The tuple is formatted") {
            out << v;
            THEN("The result is '{hello}'") {
                REQUIRE(out.str() == "{hello}");
            }
        }
    }
    GIVEN("A 3-tuple: (-1, 'hello', 'x')") {
        std::tuple<int, std::string, char> v{-1, "hello", 'x'};
        WHEN("The tuple is formatted") {
            out << v;
            THEN("The result is '{-1, hello, x}'") {
                REQUIRE(out.str() == "{-1, hello, x}");
            }
        }
    }
}