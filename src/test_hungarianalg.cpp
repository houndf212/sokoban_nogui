#include "hungarianalg.h"
#include <ctime>
#include <cstdlib>
#include "debug_print.h"

typedef HungarianAlg<IntMatrix::value_type> Hun;

IntMatrix rand_matrix()
{
    std::srand(std::time(0));
    IntMatrix m(100, 100);

    for (auto &v : m) {
        v = rand() % 100;
    }
    return m;
}


void test_HungarianAlg()
{
    IntMatrix mat = rand_matrix();
    print(mat);
    Hun h;
    h.solve(mat);
}
