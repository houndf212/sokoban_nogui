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
};

static_assert (floor1 == 0, "");

constexpr Elements_t OCCUPIED = Elements::wall | Elements::box;
constexpr Elements_t SPACE    = Elements::floor1;
constexpr Elements_t SOKOBAN  = Elements::man;
constexpr Elements_t BOX      = Elements::box;
constexpr Elements_t TARGET   = Elements::goal;
constexpr Elements_t WALL     = Elements::wall;

template<Elements e>
constexpr Elements_t EtoInt()
{
    return static_cast<Elements_t>(e);
}

inline Elements_t add_goal(Elements_t e)
{
    assert(e != Elements::wall);
    static_assert((EtoInt<Elements::floor1>()|EtoInt<Elements::goal>()) == EtoInt<Elements::goal>(),"");
    return Elements(Elements_t(e)|EtoInt<Elements::goal>());
}

#endif // ELEMENTS_H
