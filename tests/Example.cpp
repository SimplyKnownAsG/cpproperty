#include "cpproperty/Property.hpp"

#include "catch.hpp"

#include <iostream>
#include <string>

using cpproperty::Property;

class Person {
public:
    Property<Person, int, cpproperty::PUBLIC, cpproperty::PUBLIC> age;

    Person()
      : age(this){};
};

TEST_CASE("example", "[example]") {
    Person p;
    SECTION("age change") {
        p.age = 0;
        REQUIRE(0 == p.age);
        REQUIRE((p.age == 0));

        p.age = 1;
        REQUIRE(1 == p.age);

        p.age = 1 + p.age;
        REQUIRE(2 == p.age);
    }
}
