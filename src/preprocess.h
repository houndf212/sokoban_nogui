#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "types.h"

namespace PreProcess
{
    int sanity_checks(board &b);
    void close_holes_in_board(board &b);
    void remove_boxes_out_of_inner(board &b);
}

#endif // PREPROCESS_H
