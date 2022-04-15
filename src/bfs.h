#ifndef BFS_H
#define BFS_H

#include "board.h"



int mark_connectivities(const board &b, int_board &dist);

void mark_connectivities_around_place(const board &b, int_board &dist, Pos start);

void bfs_from_place(const board &b, int_board &dist, Pos start, bool ignore_boxes);

#endif // BFS_H
