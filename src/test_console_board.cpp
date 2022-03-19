#include "boardparam.h"
#include "xsb.h"
#include <iostream>
#include "debug_print.h"
using namespace std;

void test_console_board()
{
    ElementsMatrix m = XSB::from_file("board.xsb");

    print(m);

    BoardParam board;
    board.set_matrix(m);
    while (!board.is_done()) {
        char c;
        cin>>c;
        Direction d = Direction::NotValid;
        switch (c) {
        case 'w':
            d = Direction::up;
            break;
        case 's':
            d = Direction::down;
            break;
        case 'a':
            d = Direction::left;
            break;
        case 'd':
            d = Direction::right;
            break;
        default:
            break;
        }
        if (d!=Direction::NotValid && board.man_move(d)) {
            cout << "move: "<<XSB::d_to_char(d)<<endl;
        }
        print(board.to_matrix());
    }
}
