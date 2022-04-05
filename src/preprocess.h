#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "types.h"

namespace PreProcess
{
    bool sanity_checks(const board &b);
    void close_holes_in_board(board &b);
    void remove_boxes_out_of_inner(board &b);
    //把无用的box转换成 wall
    void turn_decorative_boxes_to_walls(board &b);
    bool preprocess_level(board &b);
}

#endif // PREPROCESS_H
