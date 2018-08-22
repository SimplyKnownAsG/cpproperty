#pragma once

#include "cpproperty/HasOperator.hpp"

#include <ostream>
#include <string>

namespace cpproperty {

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
            this->set(val);
        };

        Property(Property&& that)
          : self(that.self)
          , value(that.value){};

    public:
        virtual ~Property(){};

        bool operator==(const T& that) {
            return this->value == that;
        };

        Property& operator=(const Property& that) = delete;

        /** TODO: OPERATOR_STREAM
         *
         * this overload does not currently appear to be necessary.  I'm not sure if it will work
         * for all compilers to cast to the type for operator<<. It appears to on OSX/clang.
         */
        /* template<typename Parent2, typename S, Access getter2, Access setter2> */
        /* friend typename std::enable_if<getter == PUBLIC, std::ostream>::type& operator<<( */
        /*         std::ostream& stream, */
        /*         Property<Parent2, S, getter2, setter2>& p); */

    protected:
        virtual void set(T val) {
            this->value = val;
        };

        virtual T& get() {
            return this->value;
        };

#define BINARY_OP_USE(ACCESS, name, op)                                                            \
    template<typename Tthis = T, typename Tthat = T>                                               \
    typename std::enable_if<(getter == ACCESS) && (HasOperator<Tthis, Tthat>::name), Tthis>::type  \
    operator op(const Tthat& val) {                                                                \
        return this->get() op val;                                                                 \
    };                                                                                             \
                                                                                                   \
    template<typename Tthis = T, typename Tthat = T>                                               \
    const typename std::enable_if<(setter == ACCESS) && (HasOperator<Tthis, Tthat>::name),         \
                                  Tthis>::type&                                                    \
    operator op##=(const Tthat& val) {                                                             \
        /* TODO: should use get() inside set? */                                                   \
        this->set(this->value op val);                                                             \
        return this->get();                                                                        \
    };

#define LOGICAL_OP_USE(ACCESS, name, op)                                                           \
    template<typename Tthis = T, typename Tthat = T>                                               \
    typename std::enable_if<(getter == ACCESS) && (HasOperator<Tthis, Tthat>::name), bool>::type   \
    operator op(const Tthat& val) {                                                                \
        return this->get() op val;                                                                 \
    };                                                                                             \
                                                                                                   \
    template<typename Tthis = T, typename Tthat = T>                                               \
    typename std::enable_if<(getter == ACCESS) && (HasOperator<Tthis, Tthat>::name), bool>::type   \
    operator op(Tthat& val) {                                                                      \
        return this->get() op val;                                                                 \
    };

#define GETTER_SETTTER(access, ACCESS)                                                             \
    access:                                                                                        \
    /* Cannot use T within this function, but we need it to be the return type  */                 \
    /* consequently, had to define That=T                                       */                 \
    /* error without That=T was:                                                */                 \
    /*      failed requirement '(cpproperty::Access)0U == PROTECTED';           */                 \
    /*      'enable_if' cannot be used to disable this declaration              */                 \
    template<typename T2 = T, typename Tthat = T>                                                  \
    const typename std::enable_if<setter == ACCESS, T2>::type& operator=(Tthat val) {              \
        this->set(val);                                                                            \
        return this->value;                                                                        \
    };                                                                                             \
                                                                                                   \
    template<typename T2 = T>                                                                      \
    operator const typename std::enable_if<getter == ACCESS, T2>::type&() {                        \
        return this->get();                                                                        \
    };                                                                                             \
    BINARY_OP_USE(ACCESS, add, +);                                                                 \
    BINARY_OP_USE(ACCESS, sub, -);                                                                 \
    BINARY_OP_USE(ACCESS, mul, *);                                                                 \
    BINARY_OP_USE(ACCESS, div, /);                                                                 \
    BINARY_OP_USE(ACCESS, mod, %);                                                                 \
                                                                                                   \
    BINARY_OP_USE(ACCESS, band, &);                                                                \
    BINARY_OP_USE(ACCESS, bor, |);                                                                 \
    BINARY_OP_USE(ACCESS, eor, ^);                                                                 \
    BINARY_OP_USE(ACCESS, lshift, <<);                                                             \
    BINARY_OP_USE(ACCESS, rshift, >>);                                                             \
                                                                                                   \
    LOGICAL_OP_USE(ACCESS, lt, <);                                                                 \
    LOGICAL_OP_USE(ACCESS, le, <=);                                                                \
    LOGICAL_OP_USE(ACCESS, eq, ==);                                                                \
    LOGICAL_OP_USE(ACCESS, ne, !=);                                                                \
    LOGICAL_OP_USE(ACCESS, gt, >);                                                                 \
    LOGICAL_OP_USE(ACCESS, ge, >=);                                                                \
    LOGICAL_OP_USE(ACCESS, land, &&);                                                              \
    LOGICAL_OP_USE(ACCESS, lor, ||);

        GETTER_SETTTER(public, PUBLIC);
        GETTER_SETTTER(protected, PROTECTED);
    };

#undef BINARY_OP_USE
#define BINARY_OP_USE(name, op)                                                                    \
    template<typename Parent, typename T, typename Tprop, Access getter, Access setter>            \
    typename std::enable_if<(getter == PUBLIC) && (HasOperator<T, Tprop>::name), T>::type          \
    operator op(const T& val, Property<Parent, Tprop, getter, setter>& prop) {                     \
        return prop op val;                                                                        \
    };

    BINARY_OP_USE(add, +);
    BINARY_OP_USE(sub, -);
    BINARY_OP_USE(mul, *);
    BINARY_OP_USE(div, /);
    BINARY_OP_USE(mod, %);

    BINARY_OP_USE(band, &);
    BINARY_OP_USE(bor, |);
    BINARY_OP_USE(eor, ^);
    BINARY_OP_USE(lshift, <<);
    BINARY_OP_USE(rshift, >>);

#undef LOGICAL_OP_USE
#define LOGICAL_OP_USE(name, op)                                                                   \
    template<typename Parent, typename T, typename Tprop, Access getter, Access setter>            \
    typename std::enable_if<(getter == PUBLIC) && (HasOperator<T, Tprop>::name), bool>::type       \
    operator op(const T& val, Property<Parent, Tprop, getter, setter>& prop) {                     \
        /* explicit cast to prevent infinite loop */                                               \
        return ((Tprop)prop)op val;                                                                \
    };                                                                                             \
                                                                                                   \
    template<typename Parent, typename T, typename Tprop, Access getter, Access setter>            \
    typename std::enable_if<(getter == PUBLIC) && (HasOperator<T, Tprop>::name), bool>::type       \
    operator op(T& val, Property<Parent, Tprop, getter, setter>& prop) {                           \
        /* explicit cast to prevent infinite loop */                                               \
        return ((Tprop)prop)op val;                                                                \
    };

    LOGICAL_OP_USE(lt, <);
    LOGICAL_OP_USE(le, <=);
    LOGICAL_OP_USE(eq, ==);
    LOGICAL_OP_USE(ne, !=);
    LOGICAL_OP_USE(gt, >);
    LOGICAL_OP_USE(ge, >=);
    LOGICAL_OP_USE(land, &&);
    LOGICAL_OP_USE(lor, ||);

    /** TODO: search OPERATOR_STREAM
     */
    /* template<typename Parent, typename T, Access getter, Access setter> */
    /* std::ostream& operator<<(std::ostream& stream, Property<Parent, T, getter, setter>& p) { */
    /*     stream << p.get(); */
    /*     return stream; */
    /* }; */

#undef BINARY_OP_USE
#undef LOGICAL_OP_USE
}
