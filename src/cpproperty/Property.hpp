#pragma once

#include <string>

namespace cpproperty {

    enum Access { PUBLIC, PROTECTED, PRIVATE };

    template<typename Parent, typename T, Access getter = PUBLIC, Access setter = PUBLIC>
    class Property {
    private:
        friend Parent;
        Parent* self;

        T value;

        Property(Parent* self)
          : self(self){};

        Property(Property&& that)
          : self(that.self)
          , value(that.value) {}

    public:
        virtual ~Property(){};

        bool operator==(const T& that) {
            return this->value == that;
        };

        friend bool operator==(const T& that, const Property<Parent, T, getter, setter>& self) {
            return that == self.value;
        };

    protected:
        virtual T set(T val) {
            return val;
        };

        virtual T& get(T& val) {
            return val;
        };

#define GETTER_SETTTER(access, ACCESS)                                                             \
    access:                                                                                        \
    template<typename T2 = T>                                                                      \
    const typename std::enable_if<setter == ACCESS, T2>::type& operator=(T2 val) {                 \
        return this->value = this->set(val);                                                       \
    };                                                                                             \
                                                                                                   \
    template<typename T2 = T>                                                                      \
    operator const typename std::enable_if<setter == ACCESS, T2>::type&() {                        \
        return this->get(value);                                                                   \
    };

        GETTER_SETTTER(public, PUBLIC);
        GETTER_SETTTER(protected, PROTECTED);
    };
}