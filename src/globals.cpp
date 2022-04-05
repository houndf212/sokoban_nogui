#include "globals.h"
namespace globals
{

ElementsMatrix global_origin_board;
ElementsMatrix global_inner_board;
PosVector global_goals;

void setOriginBoard(const ElementsMatrix &b)
{
    global_origin_board = b;
    global_goals.clear();
    for (auto it = b.begin(); it != b.end(); ++it)
    {
        if (*it & Elements::goal)
            global_goals.push_back(it.pos());
    }
    global_goals.shrink_to_fit();

    global_inner_board = b;
    for (auto it=global_inner_board.begin(); it!=global_inner_board.end(); ++it)
    {
        Pos p = it.pos();
        auto e = *it;
        switch (e)
        {
        case Elements::wall:
            break;
        default:
            global_inner_board.set(p, Elements::floor1);
            break;
        }
    }
}
const ElementsMatrix &getOriginBoard()
{
    return global_origin_board;
}

const PosVector &getOriginGoals()
{
    return global_goals;
}

const ElementsMatrix *empty_room()
{
    return &global_inner_board;
}
}
