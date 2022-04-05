#include "board.h"
#include "globals.h"

int boardUtil::boxes_in_level(const board &b)
{
    int sum = 0;

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Elements e = b.get(Pos(i, j));
            if (static_cast<Elements_t>(e) & static_cast<Elements_t>(Elements::box))
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
            int e = int(board_without_boxes.get(p));
            Elements newE = Elements(e & ~int(Elements::box));
            board_without_boxes.set(p, newE);
        }
}

void boardUtil::expand_sokoban_cloud(board &b)
{
    int queue_y[MAX_SIZE*MAX_SIZE];
    int queue_x[MAX_SIZE*MAX_SIZE];

    int y, x, next_y, next_x;
    int queue_len = 0;
    int queue_pos = 0;

    Pos que[MAX_SIZE*MAX_SIZE];

    // init queue from existing player positions
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Pos p(i, j);
            if (EleHas(b.get(p), Elements::man))
            {
                que[queue_len] = p;
                queue_len++;
            }
        }

    while (queue_pos < queue_len)
    {
        y = queue_y[queue_pos];
        x = queue_x[queue_pos];


        Pos p = que[queue_pos];
        for (auto e : globals::deltaPos)
        {
            Pos next = globals::getPosition(p, e);
            auto ne = b.get(next);
            if ( EleHas(ne, SOKOBAN)) continue;
            if ( EleHas(ne, OCCUPIED)) continue;

            b.set(next, EleOr(ne, SOKOBAN));

            que[queue_len] = next;
            queue_len++;
        }
        queue_pos++;
    }
}


