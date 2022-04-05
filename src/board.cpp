#include "board.h"
#include "globals.h"

int boardUtil::boxes_in_level(const board &b)
{
    int sum = 0;

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Elements e = b.get(Pos(i, j));
            if (e & Elements::box)
                sum++;
        }
    return sum;
}

void boardUtil::clear_boxes(const board &b, board &board_without_boxes)
{
    copy_board(b, board_without_boxes);

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Pos p(i, j);
            Elements e = board_without_boxes.get(p);
            board_without_boxes.set(p, Elements(e & ~Elements::box));
        }
}

void boardUtil::expand_sokoban_cloud(board &b)
{
    Pos que[MAX_SIZE*MAX_SIZE];
    int queue_len = 0;
    int queue_pos = 0;

    // init queue from existing player positions
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Pos p(i, j);
            if (b.get(p) & SOKOBAN)
            {
                que[queue_len] = p;
                queue_len++;
            }
        }

    while (queue_pos < queue_len)
    {
        Pos p = que[queue_pos];
        for (auto e : globals::deltaPos)
        {
            Pos next = globals::getPosition(p, e);
            auto ne = b.get(next);
            if (ne & SOKOBAN) continue;
            if (ne & OCCUPIED) continue;

            b.set(next, Elements(ne | SOKOBAN));

            que[queue_len] = next;
            queue_len++;
        }
        queue_pos++;
    }
}


