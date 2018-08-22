#include "cpproperty/Property.hpp"

#include "catch.hpp"

#include <cmath>
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

    // unnamed class, inherits from property
    class : public Property<Particle, double, cpproperty::PUBLIC, cpproperty::PROTECTED> {
        using Property::Property;

    private:
        double& get() override {
            // sadly, "auto" does not work here...
            const double& m0 = this->self->m0;
            this->value = m0 / std::sqrt(1 - std::pow(this->self->v / 2.99792e8, 2));
            return Property::get();
        };
    } m;

    Property<Particle, double, cpproperty::PUBLIC, cpproperty::PUBLIC> v;

    Particle(double m0)
      : m0(this, m0)
      , m(this)
      , v(this, 0.0){};
};

class Particle2 {
public:
    Property<Particle2, double, cpproperty::PUBLIC, cpproperty::PROTECTED> m0;

    Property<Particle2, double, cpproperty::PUBLIC, cpproperty::PROTECTED> m;

    class : public Property<Particle2, double, cpproperty::PUBLIC, cpproperty::PUBLIC> {
    public:
        using Property::operator=;

    private:
        // I don't know if Property::Property needs to be public or private...
        using Property::Property;

        void set(double val) override {
            Property::set(val);

            // sadly, "auto" does not work here...
            const double& m0 = this->self->m0;
            this->self->m = m0 / std::sqrt(1 - std::pow(this->value / 2.99792e8, 2));
        };
    } v;

    Particle2(double m0)
      : m0(this, m0)
      , m(this)
      , v(this, 0.0){};
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
        REQUIRE(p.m == p.m0);
    }
    SECTION("particle2") {
        Particle2 p(1e-19);
        REQUIRE((p.v = 0.0) == 0.0);
        REQUIRE(p.m == p.m0);
        p.v += 1e8;
        REQUIRE(p.m > p.m0);
    }
}
