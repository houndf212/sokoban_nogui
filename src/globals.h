#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

namespace globals
{

void setOriginBoard(const ElementsMatrix &b);
const ElementsMatrix &getOriginBoard();
const PosVector &getOriginGoals();
const ElementsMatrix *empty_room();
}

#endif // GLOBALS_H
