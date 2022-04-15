#ifndef MOVES_H
#define MOVES_H

#include "board.h"

using index_t = short;
using ePos = globals::ePosition;

namespace moveNS
{
struct move
{
    index_t from; // box原来的位置 index
    index_t to; // box 推到的位置 index
    ePos    player_position; // 位置是相对推的box的位置，也就是上下左右
};

//int find_possible_moves(board b, move *moves, int *has_corral);
void apply_move(board &b, const move &m);

//void print_move(move *m);
//void get_hashes_of_moves(board b, int moves_num, move *moves, UINT_64 *hashes);

//最大50个箱子，棋盘最大 50*50， 而且棋盘外边框必须有wall， 也就是 49*49个格子， 那么最多推 50*49*49
//肯定要不到这么多，具体
#define MAX_MOVES 11500

extern int current_impossible[MAX_INNER];
extern board current_impossible_board;

}

using namespace moveNS;

#endif // MOVES_H
