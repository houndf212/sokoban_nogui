#include "boardparam.h"
#include "xsb.h"
#include "debug_print.h"
void test_boxmover()
{
    ElementsMatrix m = XSB::from_file("board.xsb");
    BoardParam param;
    param.set_matrix(m);
    print(param);

    auto lst = param.next_move();
    for (const auto &p : lst) {
        print(p);
    }

}
