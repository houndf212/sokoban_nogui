#ifndef ELEMENTS_H
#define ELEMENTS_H
#include <assert.h>

typedef unsigned char Elements_t;
enum class Elements : Elements_t
{
    man = 1<<3,  //1000
    wall = 1, //0001
    box = 1<<2,  //0100
    floor = 1<<1,//0010
    goal = wall|floor, //0011

    man_goal = man|goal, // 1011
    box_goal = box|goal, // 0111
};

constexpr Elements_t OCCUPIED = Elements_t(Elements::wall) | Elements_t(Elements::box);
constexpr Elements SOKOBAN = Elements::man;
inline
bool EleHas(Elements e1, Elements e2)
{
    return static_cast<Elements_t>(e1) & static_cast<Elements_t>(e2);
}

inline
bool EleHas(Elements e1, Elements_t e2)
{
    assert(e2 == OCCUPIED);
    return static_cast<Elements_t>(e1) & e2;
}

inline
Elements EleOr(Elements e1, Elements e2)
{
    Elements_t e = static_cast<Elements_t>(e1) | static_cast<Elements_t>(e2);
    return static_cast<Elements>(e);
}

template<Elements e>
constexpr Elements_t EtoInt()
{
    return static_cast<Elements_t>(e);
}

inline Elements add_goal(Elements e)
{
    assert(e != Elements::wall);
    static_assert((EtoInt<Elements::man>()|EtoInt<Elements::goal>()) == EtoInt<Elements::man_goal>(),"");
    static_assert((EtoInt<Elements::box>()|EtoInt<Elements::goal>()) == EtoInt<Elements::box_goal>(),"");
    static_assert((EtoInt<Elements::wall>()|EtoInt<Elements::goal>()) == EtoInt<Elements::goal>(),"");
    static_assert((EtoInt<Elements::floor>()|EtoInt<Elements::goal>()) == EtoInt<Elements::goal>(),"");
    return Elements(Elements_t(e)|EtoInt<Elements::goal>());
}

#endif // ELEMENTS_H
