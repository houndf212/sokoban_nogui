#ifndef GRAPH_H
#define GRAPH_H

#include "board.h"
#include "moves.h"

void build_graph(const board &b);

void set_graph_weights_to_infinity();
void clear_weight_around_cell(int index);
void do_graph_iterations();
int get_weight_around_cell(int index);

void find_box_moves_graph(board b, int box_index);
int get_box_moves_from_graph(int start_index, move *moves);
#endif // GRAPH_H
