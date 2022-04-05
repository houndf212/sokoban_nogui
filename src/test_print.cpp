#include "types.h"
#include "debug_print.h"
void test_print()
{
    Pos p(1, 2);
    print(p);

    ElementsMatrix m(3, 3, Elements::floor1);

    m.set(Pos(0,0), Elements::man);

    m.set(Pos(0,1), Elements::box);

    m.set(Pos(0,2), Elements::goal);

    print(m);
}
