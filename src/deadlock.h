#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "board.h"

namespace deadlock
{
void set_forbidden_tunnel();
extern FixedMatix<bool, MAX_SIZE, MAX_SIZE>  forbidden_tunnel;
}


#endif // DEADLOCK_H
