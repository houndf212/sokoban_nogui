#include "types.h"
#include <algorithm>
#include <numeric>
#include "debug_print.h"

void test_matrix_resize()
{
    IntMatrix m(10, 10);
    std::iota(m.begin(), m.end(), 0);
    print(m);


    m.resize(10, 11, 2);
    print(m);

    m.resize(6, 7, 1);
    print(m);

    m.resize(4, 7, 1);
    print(m);

    m.resize(2, 3, 1);
    print(m);
}
