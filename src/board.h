#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "util.h"
#include "fixedvector.h"
#include "debug_print.h"
#include "globals.h"

inline
void print_board(const board &b)
{ print(b); }



namespace boardUtil
{
extern  int global_boxes_in_level;
extern  Pos global_initial_player;
extern  board initial_board;
extern  BoolMatrix inner;
extern  int index_num;

inline
void copy_board(const board &from, board &to)
{
    to = from;
}
void zero_board(board &b);

int boxes_in_level(const board &b);
void get_sokoban_position(const board b, Pos &p);

void keep_boxes_in_inner(board &b);
void clear_boxes(const board &b, board &board_without_boxes);
void expand_sokoban_cloud(board &b);

void init_inner(const board &b);
void init_index_x_y();
int y_x_to_index(int y, int x);
int y_x_to_index(const Pos &p);
void index_to_y_x(int index, int *y, int *x);
void index_to_y_x(int index, Pos &p);

void save_initial_board(const board &b);
}

#endif // BOARD_H
