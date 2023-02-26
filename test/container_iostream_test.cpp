#include <catch2/catch.hpp>
#include <samwarring/container_iostream.hpp>
#include <sstream>

using namespace samwarring::container_iostream;

SCENARIO("Containers can be formatted to ostream") {
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

SCENARIO("Containers can be parsed from istream") {
    GIVEN("An input string: {1, 2, 3}") {
        std::istringstream in{"{1, 2, 3}"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("The stream state is good") {
                REQUIRE(in.good());
            }
            THEN("The vector contains 3 items") {
                REQUIRE(v.size() == 3);
            }
            THEN("The vector contains: 1, 2, 3") {
                std::vector<int> exp{1, 2, 3};
                REQUIRE(v == exp);
            }
        }
    }
    GIVEN("An input string: {}") {
        std::istringstream in{"{}"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("The stream state is good") {
                REQUIRE(in.good());
            }
            THEN("The vector is empty") {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("An input string: {    }") {
        std::istringstream in{"{    }"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("The stream state is good") {
                REQUIRE(in.good());
            }
            THEN("The vector is empty") {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("An un-terminated input string: {1, 2") {
        std::istringstream in{"{1, 2"};
        WHEN("A vector<int> initially containing elements is extracted") {
            std::vector<int> v_init{9, 9, 9};
            auto v = v_init;
            in >> v;
            THEN("The stream is in a failure state") {
                REQUIRE(in.fail());
            }
            THEN("The stream is EOF") {
                REQUIRE(in.eof());
            }
            THEN("The destination vector is not modified") {
                REQUIRE(v == v_init);
            }
        }
    }
    GIVEN("An interrupted input string: {1, 2, new stuff...") {
        std::istringstream in{"{1, 2, new stuff..."};
        WHEN("A vector<int> initially containing elements is extracted") {
            std::vector<int> v_init{9, 9, 9};
            auto v = v_init;
            in >> v;
            THEN("The stream is in a failure state") {
                REQUIRE(in.fail());
            }
            THEN("The destination vector is not modified") {
                REQUIRE(v == v_init);
            }
            THEN("The stream is not EOF") {
                REQUIRE(!in.eof());
                AND_WHEN("The next string is extracted") {
                    std::string s;
                    in.clear();
                    in >> s;
                    THEN("It extracts the un-parsed token 'new'") {
                        REQUIRE(s == "new");
                    }
                }
            }
        }
    }

    GIVEN("An input string: 1 2 3") {
        std::istringstream in{"1 2 3"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("Parsing succeeded, but stream is EOF") {
                REQUIRE(!in.fail());
                REQUIRE(in.eof());
            }
            THEN("The vector contains 3 elements") {
                REQUIRE(v.size() == 3);
            }
            THEN("The vector contains: 1, 2, 3") {
                std::vector<int> exp{1, 2, 3};
                REQUIRE(v == exp);
            }
        }
    }

    GIVEN("An input string: 1 2 three") {
        std::istringstream in{"1 2 three"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("The stream is good") {
                REQUIRE(in.good());
            }
            THEN("The vector contains: 1, 2") {
                std::vector<int> exp{1, 2};
                REQUIRE(v == exp);
            }
            AND_WHEN("The following string is extracted") {
                std::string s;
                in >> s;
                THEN("The stream is eof, but not failed") {
                    REQUIRE(in.eof());
                    REQUIRE(!in.fail());
                }
                THEN("The extracted string is: three") {
                    REQUIRE(s == "three");
                }
            }
        }
    }

    GIVEN("An input string: [1, 2  3]") {
        std::istringstream in{"[1, 2  3]"};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("The extraction failed") {
                // because the established separator ',' was not found.
                REQUIRE(in.fail());
            }
            THEN("The stream is not EOF") {
                REQUIRE(!in.eof());
            }
            THEN("The vector is not populated") {
                REQUIRE(v.empty());
            }
            AND_WHEN("The following int is extracted") {
                in.clear();
                int i;
                in >> i;
                THEN("The extracted value is 3") {
                    REQUIRE(i == 3);
                }
            }
        }
    }

    GIVEN("An input string representing an empty vector") {
        std::string input_string =
            GENERATE("[]", "{}", "()", "<>", "  []", "  <  >  ");
        CAPTURE(input_string);
        std::istringstream in{input_string};
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The vector is empty") {
                REQUIRE(v.empty());
            }
        }
    }

    GIVEN("An input string") {
        std::string input_string =
            GENERATE("1 2 3", "1, 2, 3", "1;2;3", "1 , 2 , 3", "1  ;2  ;3  ");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The vector contains: 1, 2, 3") {
                std::vector<int> exp{1, 2, 3};
                REQUIRE(v == exp);
            }
        }
    }

    GIVEN("An input string") {
        std::string input_string =
            GENERATE("[", "{ 1", "<1,", "( 1 hi", "{1, hi}");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A vector<int> is extracted") {
            std::vector<int> v;
            in >> v;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The vector is empty") {
                REQUIRE(v.empty());
            }
        }
    }
}