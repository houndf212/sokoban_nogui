#ifndef ELEMENTS_H
#define ELEMENTS_H
#include <assert.h>

typedef unsigned char Elements_t;
enum Elements : Elements_t
{
    SPACE = 0,
    WALL = 1,
    BOX = 2,
    TARGET = 4,
    SOKOBAN = 8,
    OCCUPIED = WALL | BOX,
};

#endif // ELEMENTS_H
