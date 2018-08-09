#pragma once

#include <string>

namespace cpproperty {

    namespace detail {
        // SFINAE test
        template<typename T>
        class has_operator {
#define HAS_BINARY_OP(name, op)                                                                    \
private:                                                                                           \
    template<typename C>                                                                           \
    static auto name##_(C* c1, C* c2)->decltype((*c1)op(*c2));                                     \
                                                                                                   \
    template<typename C>                                                                           \
    static void name##_(...);                                                                      \
                                                                                                   \
public:                                                                                            \
    const static int name = std::is_same<decltype(name##_<T>((T*)0, (T*)0)), T>::value;
            HAS_BINARY_OP(add, +);
            HAS_BINARY_OP(sub, -);
            HAS_BINARY_OP(mul, *);
            HAS_BINARY_OP(div, /);
        };
    };

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

#define OP_USE(ACCESS, name, op)                                                                   \
    template<typename T2 = T>                                                                      \
    typename std::enable_if<(getter == ACCESS) && (detail::has_operator<T>::name), T2>::type       \
    operator op(const T2& val) {                                                                   \
        return this->get(this->value) op val;                                                      \
    };                                                                                             \
                                                                                                   \
    template<typename T2 = T>                                                                      \
    const typename std::enable_if<(setter == ACCESS) && (detail::has_operator<T>::name),           \
                                  T2>::type&                                                       \
    operator op##=(const T2& val) {                                                                \
        return this->value = this->set(this->value op val);                                        \
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
    };                                                                                             \
    OP_USE(ACCESS, add, +);                                                                        \
    OP_USE(ACCESS, sub, -);                                                                        \
    OP_USE(ACCESS, mul, *);                                                                        \
    OP_USE(ACCESS, div, /);

        GETTER_SETTTER(public, PUBLIC);
        GETTER_SETTTER(protected, PROTECTED);
    };
}
