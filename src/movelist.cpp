#include "movelist.h"


void MoveList::check_push(Direction d)
{
//    if (!empty() && is_reverse(d, back()))
//        pop_back();
//    else
        std::list<Direction>::push_back(d);
}

void MoveList::check_append(const MoveList &lst)
{
    for (auto d : lst) {
        check_push(d);
    }
}
