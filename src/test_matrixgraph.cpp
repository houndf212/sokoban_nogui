#include "matrixgraph.h"
#include "xsb.h"
#include "boardparam.h"
#include "debug_print.h"

void test_matrixgraph()
{
    ElementsMatrix m = XSB::from_file("board.xsb");
    BoardParam start;
    start.set_matrix(m);
    print(start);

    Pos p_start = start.man();

    Direction d;
    start.man_move(d = Direction::left);
    start.man_move(d = Direction::up);
    start.man_move(d = Direction::up);
    start.man_move(d = Direction::up);
    start.man_move(d = Direction::up);
    start.man_move(d = Direction::right);
    start.man_move(d = Direction::right);
    start.man_move(d = Direction::right);
    start.man_move(d = Direction::down);
    start.man_move(d = Direction::down);

    Pos p_end = start.man();

    print(start);

    MatrixGraph g(start.cache_room());

    auto path = g.shortest_path(p_start, p_end);
    print(path);
}
