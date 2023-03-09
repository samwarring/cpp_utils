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
    GIVEN("An array with one element: 20") {
        std::array<int, 1> v{20};
        WHEN("The array is formatted") {
            out << v;
            THEN("The result is '{20}'") {
                REQUIRE(out.str() == "{20}");
            }
        }
    }
    GIVEN("An array with many elements: 1, 2, 3") {
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