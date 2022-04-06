#include "types.h"
#include "debug_print.h"
void test_print()
{
    Pos p(1, 2);
    print(p);

    ElementsMatrix m(3, 3, SPACE);

    m.set(Pos(0,0), Elements::SOKOBAN);

    m.set(Pos(0,1), Elements::BOX);

    m.set(Pos(0,2), Elements::TARGET);

    print(m);
}
