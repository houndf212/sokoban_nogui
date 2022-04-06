#ifndef ELEMENTS_H
#define ELEMENTS_H
#include <assert.h>

typedef unsigned char Elements_t;
enum Elements : Elements_t
{
    floor1 = 0,
    wall = 1,
    box = 2,
    goal = 4,
    man = 8,

    man_goal = man|goal,
    box_goal = box|goal,
};

constexpr Elements_t OCCUPIED = Elements::wall | Elements::box;
constexpr Elements SPACE    = Elements::floor1;
constexpr Elements SOKOBAN  = Elements::man;
constexpr Elements BOX      = Elements::box;
constexpr Elements TARGET   = Elements::goal;
constexpr Elements WALL     = Elements::wall;

template<Elements e>
constexpr Elements_t EtoInt()
{
    return static_cast<Elements_t>(e);
}

inline Elements_t add_goal(Elements_t e)
{
    assert(e != Elements::wall);
    static_assert((EtoInt<Elements::man>()|EtoInt<Elements::goal>()) == EtoInt<Elements::man_goal>(),"");
    static_assert((EtoInt<Elements::box>()|EtoInt<Elements::goal>()) == EtoInt<Elements::box_goal>(),"");
    static_assert((EtoInt<Elements::floor1>()|EtoInt<Elements::goal>()) == EtoInt<Elements::goal>(),"");
    return Elements(Elements_t(e)|EtoInt<Elements::goal>());
}

#endif // ELEMENTS_H
