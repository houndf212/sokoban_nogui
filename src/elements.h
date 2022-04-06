#ifndef ELEMENTS_H
#define ELEMENTS_H
#include <assert.h>

typedef unsigned char Elements_t;
enum Elements : Elements_t
{
    wall = 1,
    box = 2,
    goal = 4,
    man = 8,
};

constexpr Elements_t SPACE    = 0;
constexpr Elements_t WALL     = 1;
constexpr Elements_t BOX      = 2;
constexpr Elements_t TARGET   = 4;
constexpr Elements_t SOKOBAN  = 8;

constexpr Elements_t OCCUPIED = WALL | BOX;


template<Elements_t e>
constexpr Elements_t EtoInt()
{
    return static_cast<Elements_t>(e);
}

inline Elements_t add_goal(Elements_t e)
{
    assert(e != Elements::wall);
    static_assert((EtoInt<SPACE>()|EtoInt<Elements::goal>()) == EtoInt<Elements::goal>(),"");
    return Elements(Elements_t(e)|EtoInt<Elements::goal>());
}

#endif // ELEMENTS_H
