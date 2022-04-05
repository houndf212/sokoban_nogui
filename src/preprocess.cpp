#include "preprocess.h"
#include "globals.h"
#include "debug_print.h"

int PreProcess::sanity_checks(board &b)
{
    int box_num = boxes_in_level(b);

    if (box_num >= MAX_BOXES)
    {
        printf("%d boxes\n", box_num);
        printf("too many boxes\n");
        sprintf(global_fail_reason, "Too many boxes");
        return 0;
    }
    return 1;
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
                if (b.get(p) != Elements::floor) continue;

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
}


