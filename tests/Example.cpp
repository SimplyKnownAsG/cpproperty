#include "cpproperty/Property.hpp"

#include "catch.hpp"

#include <iostream>
#include <string>

using cpproperty::Property;

class Person {
public:
    Property<Person, int, cpproperty::PUBLIC, cpproperty::PUBLIC> age;

    Property<Person, std::string, cpproperty::PUBLIC, cpproperty::PUBLIC> name;

    Person()
      : age(this)
      , name(this){};
};

TEST_CASE("example", "[example]") {
    Person p;
    SECTION("age change") {
        p.age = 0;
        REQUIRE(0 == p.age);
        REQUIRE((p.age == 0));

        p.age = 1;
        REQUIRE(1 == p.age);

        p.age = p.age + 1;
        REQUIRE(2 == p.age);

        // compiler error on Linux?
        p.age = 1 + p.age;
        REQUIRE(3 == p.age);

        p.age += 1;
        REQUIRE(4 == p.age);

        p.age *= 2;
        REQUIRE(8 == p.age);

        p.age -= 2;
        REQUIRE(6 == p.age);

        p.age /= 2;
        REQUIRE(3 == p.age);
    }
    SECTION("strings") {
        p.name = "hello";
        REQUIRE((p.name == "hello"));

        // compiler error
        p.name = "goodbye";
    }
}
