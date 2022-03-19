#ifndef MOVELIST_H
#define MOVELIST_H
#include <list>
#include "movedirection.h"

class MoveList : public std::list<Direction>
{
public:
    void check_push(Direction d);
    void check_append(const MoveList &lst);
private:
    void push_back();
};

#endif // MOVELIST_H
