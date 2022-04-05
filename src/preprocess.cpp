#include "preprocess.h"
#include "globals.h"
#include "debug_print.h"
#include "board.h"


bool PreProcess::sanity_checks(const board &b)
{
    int box_num = boardUtil::boxes_in_level(b);

    if (box_num >= MAX_BOXES)
    {
        printf("%d boxes\n", box_num);
        util::exit_with_error("too many boxes\n");
        return false;
    }
    return true;
}

void PreProcess::close_holes_in_board(board &b)
{
    assert(b.row_size() >=2 && b.col_size()>=2);

    do
    {
        bool changed = false;
        for (int r=1; r<b.row_size()-1; ++r)
            for (int c=1; c<b.col_size()-1; ++c)
            {
                Pos p(r, c);
                if (b.get(p) != Elements::floor1) continue;

                int sum = 0;
                for (auto e : globals::deltaPos)
                {
                    Pos p1 = globals::getPosition(p, e);
                    if (b.get(p1) == Elements::wall)
                        sum++;
                }

                if (sum >= 3)
                {
                    b.set(p, Elements::wall);
                    changed = true;
                }
            }

        if (!changed)
            return;

    }while(true);
}

void PreProcess::remove_boxes_out_of_inner(board &b)
{
    board inner;
    boardUtil::clear_boxes(b, inner);
    boardUtil::expand_sokoban_cloud(inner);

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
            if ((inner.get(i, j) & SOKOBAN) == 0)
            {
                Elements_t e = inner.get(i, j);
                e &= ~BOX;
                e &= ~TARGET;
                b.set(i, j, Elements(e));
            }
}
