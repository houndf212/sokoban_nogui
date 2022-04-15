#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "util.h"
#include "fixedvector.h"
#include "debug_print.h"
#include "globals.h"


using int_board = FixedMatix<int, MAX_SIZE, MAX_SIZE>;

inline
void print_board(const board &b)
{ print(b); }



namespace boardUtil
{
extern  int global_boxes_in_level;
extern  Pos global_initial_player;
extern  board initial_board;
extern  FixedMatix<bool, MAX_SIZE, MAX_SIZE> inner;
extern  int index_num;

inline bool isInInner(Pos p) { return inner.at(p); }

inline
void copy_board(const board &from, board &to)
{
    to = from;
}
void zero_board(board &b);

int boxes_in_level(const board &b);
void get_sokoban_position(const board b, Pos &p);
void clear_sokoban_inplace(board &b);
void keep_boxes_in_inner(board &b);
void clear_boxes(const board &b, board &board_without_boxes);
bool board_contains_boxes(const board &b);
void expand_sokoban_cloud(board &b);
void expand_sokoban_cloud_one_sokoban(board &b, Pos p);

void init_inner(const board &b);
void init_index_x_y();
//int y_x_to_index(int y, int x);
int y_x_to_index(const Pos &p);
//void index_to_y_x(int index, int *y, int *x);
void index_to_y_x(int index, Pos &p);

void save_initial_board(const board &b);

template<FIXED_SIZE_TYPE _R, FIXED_SIZE_TYPE _C>
void zero_board_bool(FixedMatix<bool, _R, _C> &m, int row, int col)
{
    for (int i=0; i<row; ++i)
        for (int j=0; j<col; ++j)
            m.at(i, j) = false;
}

}

using namespace boardUtil;

#endif // BOARD_H
