#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "util.h"

namespace boardUtil
{
int boxes_in_level(const board &b);
inline
void copy_board(const board &from, board &to)
{
    to = from;
}
void clear_boxes(const board &b, board &board_without_boxes);
void expand_sokoban_cloud(board &b);
}

#endif // BOARD_H
