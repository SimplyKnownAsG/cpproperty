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

class Particle {
public:
    Property<Particle, double, cpproperty::PUBLIC, cpproperty::PROTECTED> m0;

    class MASS : public Property<Particle, double, cpproperty::PUBLIC, cpproperty::PUBLIC> {
        friend class Particle;
        using Property::Property;
        double& get(double& val) {
            // sadly, "auto" does not work here...
            const double& m0 = this->self->m0;
            this->value = m0 / std::sqrt(1 - std::pow(this->self->v / 2.99792e8, 2));
            return this->value;
        };
    } m;

    Property<Particle, double, cpproperty::PUBLIC, cpproperty::PUBLIC> v;

    Particle(double m0)
      : m0(this, m0)
      , m(this)
      , v(this){};
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
    SECTION("particle") {
        Particle p(1e-19);
        REQUIRE((p.v = 0.0) == 0.0);
        std::cout << "p.m0: " << p.m0 << std::endl;
        std::cout << "p.m: " << p.m << std::endl;
        REQUIRE(p.m == p.m0);
    }
}
