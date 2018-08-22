#pragma once

#include <ostream>
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

        /* template<typename Parent2, typename S, Access getter2, Access setter2> */
        /* friend class std::enable_if<std::is_same<Parent2, Parent>::value, */
        /*                             Property<Parent2, S, getter2, setter2>>::type; */

        Parent* self;

        T value;

        Property(Parent* self)
          : self(self){};

        Property(Parent* self, T val)
          : self(self) {
            this->value = this->set(val);
        };

        Property(Property&& that)
          : self(that.self)
          , value(that.value){};

    public:
        virtual ~Property(){};

        bool operator==(const T& that) {
            return this->value == that;
        };

        /** XXX: OPERATOR_STREAM
         *
         * this overload does not currently appear to be necessary.  I'm not sure if it will work
         * for all compilers to cast to the type for operator<<. It appears to on OSX/clang.
         */
        /* template<typename Parent2, typename S, Access getter2, Access setter2> */
        /* friend typename std::enable_if<getter == PUBLIC, std::ostream>::type& operator<<( */
        /*         std::ostream& stream, */
        /*         Property<Parent2, S, getter2, setter2>& p); */

    protected:
        virtual T set(T val) {
            return val;
        };

        virtual T& get(T& val) {
            return val;
        };

#define OP_USE(ACCESS, name, op)                                                                   \
    template<typename T2 = T>                                                                      \
    typename std::enable_if<(getter == ACCESS) && (detail::has_operator<T2>::name), T2>::type      \
    operator op(const T2& val) {                                                                   \
        return this->get(this->value) op val;                                                      \
    };                                                                                             \
                                                                                                   \
    template<typename T2 = T>                                                                      \
    const typename std::enable_if<(setter == ACCESS) && (detail::has_operator<T2>::name),          \
                                  T2>::type&                                                       \
    operator op##=(const T2& val) {                                                                \
        return this->value = this->set(this->get(this->value) op val);                             \
    };

#define GETTER_SETTTER(access, ACCESS)                                                             \
    access:                                                                                        \
    /* Cannot use T within this function, but we need it to be the return type  */                 \
    /* consequently, had to define Treturn=T                                    */                 \
    /* error without Treturn=T was:                                             */                 \
    /*      failed requirement '(cpproperty::Access)0U == PROTECTED';           */                 \
    /*      'enable_if' cannot be used to disable this declaration              */                 \
    template<typename T2 = T, typename Treturn = T>                                                \
    const typename std::enable_if<setter == ACCESS, Treturn>::type& operator=(T2 val) {            \
        return this->value = this->set(val);                                                       \
    };                                                                                             \
                                                                                                   \
    template<typename T2 = T>                                                                      \
    operator const typename std::enable_if<getter == ACCESS, T2>::type&() {                        \
        return this->get(value);                                                                   \
    };                                                                                             \
    OP_USE(ACCESS, add, +);                                                                        \
    OP_USE(ACCESS, sub, -);                                                                        \
    OP_USE(ACCESS, mul, *);                                                                        \
    OP_USE(ACCESS, div, /);

        GETTER_SETTTER(public, PUBLIC);
        GETTER_SETTTER(protected, PROTECTED);
    };

#undef OP_USE
#define OP_USE(name, op)                                                                           \
    template<typename Parent, typename T, Access getter, Access setter>                            \
    typename std::enable_if<(getter == PUBLIC) && (detail::has_operator<T>::name), T>::type        \
    operator op(const T& val, Property<Parent, T, getter, setter>& prop) {                         \
        return prop op val;                                                                        \
    };

    OP_USE(add, +);
    OP_USE(sub, -);
    OP_USE(mul, *);
    OP_USE(div, /);

    /** XXX: search OPERATOR_STREAM
     */
    /* template<typename Parent, typename T, Access getter, Access setter> */
    /* std::ostream& operator<<(std::ostream& stream, Property<Parent, T, getter, setter>& p) { */
    /*     stream << p.get(p.value); */
    /*     return stream; */
    /* }; */
}
