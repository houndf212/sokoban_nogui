#ifndef ROOMSLICE_H
#define ROOMSLICE_H
#include "boardparam.h"

class RoomSlice
{
public:
    RoomSlice(const BoardParam &pa);
    bool can_man_to(Pos p) const;
private:
    static constexpr IntMatrix::value_type k_block = 0;
    static constexpr IntMatrix::value_type k_unflagged = -1;

    void seedPos(Pos p, IntMatrix::value_type val);
private:
    const BoardParam &param;
    IntMatrix slice;
};

#endif // ROOMSLICE_H
