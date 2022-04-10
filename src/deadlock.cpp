#include "deadlock.h"

FixedMatix<bool, MAX_SIZE, MAX_SIZE> deadlock::forbidden_tunnel;


void zero_board_bool(FixedMatix<bool, MAX_SIZE, MAX_SIZE> &m, int row, int col)
{
    for (int i=0; i<row; ++i)
        for (int j=0; j<col; ++j)
            m.at(i, j) = false;
}

//
/* forbidden_tunnel 不是说不能进入，意思是 一个局面的box不能进入，因为box在 tunnel中没有任何意义，
 * 总归是要推出来，除非target在 tunnel，在函数 ‘is_inefficient_position’中应用，显示判断target
 *
 *  #######
 *  11$1111
 *  #######
*/

void deadlock::set_forbidden_tunnel()
{
    int height = initial_board.row_size();
    int width  = initial_board.col_size();

    board b;

    zero_board_bool(forbidden_tunnel, height, width);

    clear_boxes(initial_board, b);
    clear_sokoban_inplace(b);

    // look for
    // ######
    //
    // ######
    for (int i = 1; i < (height-1); i++)
    {
        for (int j = 0; j < width; j++)
        {
            int k = 0;
            for (; k < width; k++)
            {
                if (b.get(i - 1, j + k) != WALL) break;
                if (b.get(i    , j + k) != SPACE) break;
                if (b.get(i + 1, j + k) != WALL) break;
            }
            if (k >= 2)
            {
                int n = k;

                int base = -1;
                // look for base
                for (k = 0; k < n; k++)
                    if (initial_board.get(i, j + k) & BOX)
                        break;

                if (k < n)
                    base = k;

                for (k = 0; k < n; k++)
                {
                    forbidden_tunnel.at(i, j + k) = true;
                    b.at(i, j + k) |= SOKOBAN; //标记sokoban意思可能是为了下次循环不用进相同的tunnel？
                }

                //这里如果 通道中有box，那么选取box为可以进入的，这样初始化位置就不会是 禁止的了，虽然可能多推box，
                //但是可以避免初始化位置不合法
                if (base == -1)
                    forbidden_tunnel.at(i, j) = false;
                else
                    forbidden_tunnel.at(i,j + base) = false;
            }
        }
    }

    // look for
    // # #
    // # #
    // # #
    for (int i = 0; i < height; i++)
    {
        for (int j = 1; j < (width - 1); j++)
        {
            int k = 0;
            for (; k < height; k++)
            {
                if (b.at(i + k, j - 1) != WALL) break;
                if (b.at(i + k, j    ) != SPACE) break;
                if (b.at(i + k, j + 1) != WALL) break;
            }
            if (k >= 2)
            {
                int n = k;

                int base = -1;
                for (k = 0; k < n; k++)
                    if (initial_board.at(i + k, j) & BOX)
                        break;

                if (k < n)
                    base = k;

                for (k = 0; k < n; k++)
                {
                    forbidden_tunnel.at(i + k, j) = true;
                    b.at(i + k, j) |= SOKOBAN;
                }

                if (base == -1)
                    forbidden_tunnel.at(i, j) = false;
                else
                    forbidden_tunnel.at(i + base, j) = false;
            }
        }
    }
}
