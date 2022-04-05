#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

namespace globals
{

constexpr int MAX_SIZE = 51;
constexpr int MAX_BOXES = 50;
constexpr int MAX_INNER  = MAX_SIZE*MAX_SIZE;

void setOriginBoard(const ElementsMatrix &b);
const ElementsMatrix &getOriginBoard();
const PosVector &getOriginGoals();
const ElementsMatrix *empty_room();

enum struct ePosition
{
    eUp = 0,
    eRight = 1,
    eDown = 2,
    eLeft = 3
};

//int delta_y[4] = { -1, 0, 1, 0 };
//int delta_x[4] = { 0, 1, 0, -1 };
/*
*     0
*    3$1
*     2
*/
constexpr int deltaPosSize = 4;
constexpr ePosition deltaPos[deltaPosSize] = {ePosition::eUp, ePosition::eRight, ePosition::eDown, ePosition::eLeft};
constexpr Pos gFourPos[deltaPosSize] = {Pos(-1, 0), Pos(-1, 0), Pos(-1, 0), Pos(-1, 0)};
inline void pos_add(Pos &p1, Pos p2) { p1.row() += p2.row(); p1.col() += p2.col(); }
inline Pos getPosition(Pos p, ePosition e) { pos_add(p, gFourPos[int(e)]); return p; }
}

using globals::MAX_SIZE;
using globals::MAX_BOXES;
using globals::MAX_INNER;

#endif // GLOBALS_H
