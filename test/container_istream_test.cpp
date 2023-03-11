#include <catch2/catch.hpp>
#include <samwarring/container_iostream.hpp>
#include <sstream>

using namespace samwarring::container_istream;
using namespace Catch::Matchers;

TEMPLATE_TEST_CASE("Scenario: Dynamic containers can be parsed from istream",
                   "", std::vector<int>, std::list<int>, std::set<int>,
                   std::unordered_set<int>, std::deque<int>) {
    GIVEN("An input string representing an empty sequence") {
        std::string input_string =
            GENERATE("[]", "{}", "()", "<>", "  []", "  <  >  ");
        CAPTURE(input_string);
        std::istringstream in{input_string};
        WHEN("A container is extracted") {
            TestType v = GENERATE(TestType{}, TestType{1, 2, 3});
            CAPTURE(v);
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The container is empty") {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("An input string representing the sequence 1 2 3") {
        std::string input_string = GENERATE(
            "1 2 3", "1, 2, 3", "1;2;3", "1, 2, 3,", "1 , 2 , 3", "1  ;2  ;3  ",
            "[1,2,3]", "{1;2;3}", "< 1 2 3 >", "   (1  ,2  ,3  )");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A container is extracted") {
            TestType v = GENERATE(TestType{}, TestType{7, 8, 9});
            CAPTURE(v);
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The container contains: 1, 2, 3") {
                TestType exp{1, 2, 3};
                REQUIRE(v == exp);
            }
        }
    }
    GIVEN("A string containing an enclosed sequence interrupted by EOF") {
        std::string input_string = GENERATE("[", "{1", "< 1,", "(1 2   ");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A container is extracted") {
            TestType v;
            in >> v;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The stream is EOF") {
                REQUIRE(in.eof());
            }
            THEN("The container is empty") {
                REQUIRE(v.empty());
            }
        }
    }
    GIVEN("A string containing an enclosed sequence of ints interrupted by a "
          "string \'foo\'") {
        std::string input_string =
            GENERATE("[foo", "[1 foo", "[1, foo", "[1foo");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A container is extracted") {
            TestType v;
            in >> v;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The stream is not EOF") {
                REQUIRE(!in.eof());
            }
            THEN("The container is empty") {
                REQUIRE(v.empty());
            }
            AND_WHEN("The following string is extracted") {
                in.clear();
                std::string following_string;
                in >> following_string;
                THEN("The extracted string is \'foo\'") {
                    REQUIRE(following_string == "foo");
                }
            }
        }
    }
    GIVEN("An open sequence interrupted by the string \'foo\'") {
        std::string input_string =
            GENERATE("1 2 foo", "1, 2, foo", "1 ;2 ;foo");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A container is extracted") {
            TestType v;
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(in.good());
            }
            THEN("The container contains the available sequence") {
                TestType exp{1, 2};
                REQUIRE(v == exp);
            }
            AND_WHEN("The following string is extracted") {
                std::string following_string;
                in >> following_string;
                THEN("The follosing string is \'foo\'") {
                    REQUIRE(following_string == "foo");
                }
            }
        }
    }
    GIVEN("A string not containing any sequence of ints") {
        std::string input_string = GENERATE("", "foo", "  foo");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A container is extracted") {
            TestType v;
            in >> v;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The container is empty") {
                REQUIRE(v.empty());
            }
        }
    }
}

SCENARIO("Containers of containers can be parsed from istream") {
    GIVEN("A complicated input string") {
        std::string input_string{"[{1, 2, 3} {7 8 8 9} {-1; 0; 1}]"};
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A vector<set<int>> is parsed") {
            std::vector<std::set<int>> v;
            in >> v;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The vector contains 3 items") {
                REQUIRE(v.size() == 3);
                AND_THEN("The 0th set contains: 1, 2, 3") {
                    std::set<int> exp{1, 2, 3};
                    REQUIRE(v[0] == exp);
                }
                AND_THEN("The 1st set contains: 7, 8, 9") {
                    std::set<int> exp{7, 8, 9};
                    REQUIRE(v[1] == exp);
                }
                AND_THEN("The 2nd set contains: -1, 0, 1") {
                    std::set<int> exp{-1, 0, 1};
                    REQUIRE(v[2] == exp);
                }
            }
        }
    }
}

SCENARIO("std::array can be parsed from istream") {
    GIVEN("An empty string") {
        std::istringstream in;
        WHEN("An array<int, 0> is parsed") {
            std::array<int, 0> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
        WHEN("An array<int, 1> is parsed") {
            std::array<int, 1> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
    }
    GIVEN("A string representing an empty array") {
        std::string input_string = GENERATE("[]", "{ }", " <> ", "  (  )");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("An array<int, 0> is parsed") {
            std::array<int, 0> arr;
            in >> arr;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
        }
        WHEN("An array<int, 1> is parsed") {
            std::array<int, 1> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
    }
    GIVEN("A string representing an array with one value") {
        std::string input_string =
            GENERATE("42", "[42]", "{42,}", "< 42 ; >", "( 42 ,)", "[ 42, ]");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("An array<int, 0> is parsed") {
            std::array<int, 0> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
        WHEN("An array<int, 1> is parsed") {
            std::array<int, 1> arr;
            in >> arr;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The array contains the parsed value") {
                REQUIRE(arr[0] == 42);
            }
        }
        WHEN("An array<int, 2> is parsed") {
            std::array<int, 2> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
    }
    GIVEN("A string representing an open array with 3 values and no trailing "
          "separator") {
        std::string input_string =
            GENERATE("1 2 3", "1,2,3", "1, 2, 3", "1 ;2 ;3");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("An array<int, 2> is parsed") {
            std::array<int, 2> arr;
            in >> arr;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The array contains the first 2 values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
            }
        }
        WHEN("An array<int, 3> is parsed") {
            std::array<int, 3> arr;
            in >> arr;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The array contains the values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
                REQUIRE(arr[2] == 3);
            }
            THEN("The stream is EOF") {
                // Would not be the case if there is a trailing separator
                REQUIRE(in.eof());
            }
        }
        WHEN("An array<int, 4> is parsed") {
            std::array<int, 4> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The array begins with the 3 parsed values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
                REQUIRE(arr[2] == 3);
            }
            THEN("The stream is EOF") {
                REQUIRE(in.eof());
            }
        }
    }
    GIVEN("A string representing an enclosed array with 3 values") {
        std::string input_string =
            GENERATE("[1 2 3]", "{ 1,2,3 }", "<1, 2, 3>", "(1 ;2 ;3  )");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("An array<int, 2> is parsed") {
            std::array<int, 2> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The array contains the first 2 values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
            }
        }
        WHEN("An array<int, 3> is parsed") {
            std::array<int, 3> arr;
            in >> arr;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The array contains the values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
                REQUIRE(arr[2] == 3);
            }
        }
        WHEN("An array<int, 4> is parsed") {
            std::array<int, 4> arr;
            in >> arr;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
            THEN("The array begins with the 3 parsed values") {
                REQUIRE(arr[0] == 1);
                REQUIRE(arr[1] == 2);
                REQUIRE(arr[2] == 3);
            }
        }
    }
}

TEMPLATE_TEST_CASE("Scenario: Maps can be parsed from istream", "",
                   (std::map<int, int>), (std::unordered_map<int, int>)) {
    GIVEN("A string representing an empty map") {
        std::string input_string = GENERATE("()", "[ ]", "{  }", "  <  >  ");
        CAPTURE(input_string);
        std::istringstream in{input_string};
        WHEN("A map is parsed") {
            TestType m;
            in >> m;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The parsed map is empty") {
                REQUIRE(m.empty());
            }
        }
    }
    GIVEN("A map containing a single entry: {6: -42}") {
        std::string input_string =
            GENERATE("{6:-42}", "{ 6 : -42 }", "6:-42", "[6: -42,]");
        CAPTURE(input_string);
        std::istringstream in{input_string};
        WHEN("A map is parsed") {
            TestType m;
            in >> m;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The map contains 1 element") {
                REQUIRE(m.size() == 1);
                AND_THEN("The map contains a key for 6") {
                    auto it = m.find(6);
                    REQUIRE(it != m.end());
                    AND_THEN("The key 6 maps to value -42") {
                        REQUIRE(it->second == -42);
                    }
                }
            }
        }
    }
}

SCENARIO("Pairs can be parsed from istream") {
    GIVEN("A string representing a pair of ints: 1, 2") {
        std::string input_string =
            GENERATE("1, 2", " 1   2 ", "[1;2]", "1,2", "{1, 2}");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A pair<int,int> is parsed") {
            std::pair<int, int> p;
            in >> p;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The pair contains: 1, 2") {
                REQUIRE(p.first == 1);
                REQUIRE(p.second == 2);
            }
        }
    }
}

SCENARIO("Tuples can be parsed from istream") {
    GIVEN("A string representing an empty tuple") {
        std::string input_string = GENERATE("[]", "{ }", " <  >  ", "()");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("An empty-tuple is parsed") {
            std::tuple<> t;
            in >> t;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
        }
        WHEN("A tuple<int> is parsed") {
            std::tuple<int> t;
            in >> t;
            THEN("Parsing fails") {
                REQUIRE(in.fail());
            }
        }
    }
    GIVEN("A string representing an int-float-int tuple: 1, 3.14, 5") {
        std::string input_string =
            GENERATE("1 3.14 5", "{1, 3.14, 5}", "[1;3.14;5;]");
        CAPTURE(input_string);
        std::istringstream in(input_string);
        WHEN("A tuple<int, float, int> is extracted") {
            std::tuple<int, float, int> t;
            in >> t;
            THEN("Parsing succeeds") {
                REQUIRE(!in.fail());
            }
            THEN("The tuple contains: 1, 3.14, 5") {
                REQUIRE(std::get<0>(t) == 1);
                REQUIRE_THAT(std::get<1>(t), WithinRel(3.14f));
                REQUIRE(std::get<2>(t) == 5);
            }
        }
    }
}