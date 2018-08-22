#pragma once

#include <ostream>
#include <string>

namespace cpproperty {

    // SFINAE test
    template<typename Tleft, typename Tright>
    class HasOperator {
#define HAS_BINARY_OP(name, op)                                                                    \
private:                                                                                           \
    template<typename Sleft, typename Sright>                                                      \
    static auto name##_(Sleft* left, Sright* right)->decltype((*left)op(*right));                  \
                                                                                                   \
    template<typename Sleft, typename Sright>                                                      \
    static void name##_(...);                                                                      \
                                                                                                   \
public:                                                                                            \
    const static int name =                                                                        \
            std::is_same<decltype(name##_<Tleft, Tright>((Tleft*)0, (Tright*)0)), Tleft>::value;
        HAS_BINARY_OP(add, +);
        HAS_BINARY_OP(sub, -);
        HAS_BINARY_OP(mul, *);
        HAS_BINARY_OP(div, /);
        HAS_BINARY_OP(mod, %);

        HAS_BINARY_OP(band, &);
        HAS_BINARY_OP(bor, |);
        HAS_BINARY_OP(eor, ^); // xor is a reserved word?
        HAS_BINARY_OP(lshift, <<);
        HAS_BINARY_OP(rshift, >>);

#define HAS_LOGICAL_OP(name, op)                                                                   \
private:                                                                                           \
    template<typename Sleft, typename Sright>                                                      \
    static auto name##_(Sleft* left, Sright* right)->decltype((*left)op(*right));                  \
                                                                                                   \
    template<typename Sleft, typename Sright>                                                      \
    static void name##_(...);                                                                      \
                                                                                                   \
public:                                                                                            \
    const static int name =                                                                        \
            std::is_same<decltype(name##_<Tleft, Tright>((Tleft*)0, (Tright*)0)), bool>::value;

        HAS_LOGICAL_OP(lt, <);
        HAS_LOGICAL_OP(le, <=);
        HAS_LOGICAL_OP(eq, ==);
        HAS_LOGICAL_OP(ne, !=);
        HAS_LOGICAL_OP(gt, >);
        HAS_LOGICAL_OP(ge, >=);
        HAS_LOGICAL_OP(land, &&);
        HAS_LOGICAL_OP(lor, ||);
    };
}
