#include "globals.h"
namespace globals
{

ElementsMatrix global_origin_board;
PosVector global_goals;

void setOriginBoard(const ElementsMatrix &b)
{
    global_origin_board = b;
    for (auto it = b.begin(); it != b.end(); ++it)
    {
        if (*it == Elements::goal)
            global_goals.push_back(it.pos());
    }
    global_goals.shrink_to_fit();
}
const ElementsMatrix &getOriginBoard()
{
    return global_origin_board;
}

const PosVector &getOriginGoals()
{
    return global_goals;
}
}
